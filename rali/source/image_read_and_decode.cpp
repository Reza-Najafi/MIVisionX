
#include <iterator>
#include <cstring>
#include "decoder_factory.h"
#include "image_read_and_decode.h"

#define DBG_TIMING 1 // Enables timings for debug purposes, timings will get printed after loading all the files

#define NUMSF  16
static const tjscalingfactor sf[NUMSF] = {
  { 2, 1 },
  { 15, 8 },
  { 7, 4 },
  { 13, 8 },
  { 3, 2 },
  { 11, 8 },
  { 5, 4 },
  { 9, 8 },
  { 1, 1 },
  { 7, 8 },
  { 3, 4 },
  { 5, 8 },
  { 1, 2 },
  { 3, 8 },
  { 1, 4 },
  { 1, 8 }
};

std::tuple<Decoder::ColorFormat, unsigned > 
interpret_color_format(RaliColorFormat color_format ) 
{
    switch (color_format) {
        case RaliColorFormat::RGB24:
            return  std::make_tuple(Decoder::ColorFormat::RGB, 3);

        case RaliColorFormat::BGR24:
            return  std::make_tuple(Decoder::ColorFormat::BGR, 3);

        case RaliColorFormat::U8:
            return  std::make_tuple(Decoder::ColorFormat::GRAY, 1);

        default:
            throw std::invalid_argument("Invalid color format\n");
    }    
}



std::vector<long long unsigned>
ImageReadAndDecode::timing()
{
    return {  _file_load_time.get_timing() , _decode_time.get_timing()};
}

ImageReadAndDecode::ImageReadAndDecode():
    _file_load_time("FileLoadTime", DBG_TIMING ),
    _decode_time("DecodeTime", DBG_TIMING)
{
}

ImageReadAndDecode::~ImageReadAndDecode()
{
    _reader = nullptr;
    _decoder = nullptr;
}   

void
ImageReadAndDecode::create(ReaderConfig reader_config, DecoderConfig decoder_config)
{
    // Can initialize it to any decoder types if needed
    _compressed_buff.resize(MAX_COMPRESSED_SIZE);
    _decoder = create_decoder(decoder_config);
    _reader = create_reader(reader_config);
}

void 
ImageReadAndDecode::reset()
{
    // TODO: Reload images from the folder if needed
    _reader->reset();
}

size_t
ImageReadAndDecode::count()
{
    return _reader->count();
}

LoaderModuleStatus 
ImageReadAndDecode::load(unsigned char* buff,
                         std::vector<std::string>& names,
                         unsigned batch_size,
                         unsigned output_width,
                         unsigned output_height,
                         std::vector<uint>& decoded_width,
                         std::vector<uint>& decoded_height,
                         RaliColorFormat output_color_format )
{
    if(output_width == 0 || output_height == 0 )
        THROW("Zero image dimension is not valid")
    if(batch_size == 0)
        THROW("Batch size 0 is not valid")
    if(!buff)
        THROW("Null pointer passed as output buffer")
    if(_reader->count() < batch_size)
        return LoaderModuleStatus::NO_MORE_DATA_TO_READ;
    // load images/frames from the disk and push them as a large image onto the buff
    unsigned file_counter = 0;

    auto [decoder_color_format, output_planes] = interpret_color_format(output_color_format);
    // Decode with the height and size equal to a single image  
    size_t single_image_height = output_height/batch_size;
    size_t single_image_size = output_width*single_image_height*output_planes;

    while ((file_counter != batch_size) && _reader->count() > 0) 
    {

        _file_load_time.start();// Debug timing

        size_t fsize = _reader->open();

        if( fsize == 0 )
        {
            WRN("Opened file "+ _reader->id()+ " of size 0");
            continue;
        }

        if( fsize > MAX_COMPRESSED_SIZE ) 
        {
            _reader->close();
            WRN("File "+ _reader->id( )+ "is larger than max "+TOSTR(MAX_COMPRESSED_SIZE)+" bytes , skipped the file");
            continue;
        }

        auto actual_read_size = _reader->read(_compressed_buff.data(), fsize);
        auto image_name = _reader->id();
        _reader->close();
        _file_load_time.end();// Debug timing


        _decode_time.start();// Debug timing
        int width, height, jpeg_sub_samp;
        if(_decoder->decode_info(_compressed_buff.data(), actual_read_size, &width, &height, &jpeg_sub_samp ) != Decoder::Status::OK)
        {
            WRN("Could not decode the header of the: "+ _reader->id())
            continue;
        }

        // Images are stacked on top of each other, offset defines 
        // where in the buffer the data of a new image starts
        size_t Image_buff_offset = single_image_size*file_counter;
        if(decode( _compressed_buff.data(), 
                fsize, 
                buff+Image_buff_offset, 
                output_width, 
                single_image_height, 
                decoder_color_format, 
                output_planes) != LoaderModuleStatus::OK)
        {
            continue;
        }

        uint scaledw, scaledh;

        if (output_width == 0) output_width = width;
        if (single_image_height == 0) single_image_height = height;
        for (int i = 0; i < NUMSF; i++) {
            scaledw = TJSCALED(width, sf[i]);
            scaledh = TJSCALED(height, sf[i]);
            if (scaledw <= output_width && scaledh <= single_image_height){
               break;
            }
        }

        _decode_time.end();// Debug timing
        names[file_counter] = image_name;
        decoded_width[file_counter] = scaledw;
        decoded_height[file_counter] = scaledh;
        file_counter++;
    }

    return LoaderModuleStatus::OK;
}

LoaderModuleStatus ImageReadAndDecode::decode(unsigned char* input_buff,
                                              size_t size,
                                              unsigned char *output_buff,
                                              unsigned output_width,
                                              unsigned output_height,
                                              Decoder::ColorFormat color_format,
                                              unsigned output_planes)
{


    int jpeg_sub_samp;
    int wd, ht;

    
    if(_decoder->decode_info(input_buff, size, &wd, &ht, &jpeg_sub_samp ) != Decoder::Status::OK) 
        return LoaderModuleStatus::DECODE_FAILED;

    
    if((unsigned)wd != output_width || (unsigned)ht != output_height) {
        // Seeting the whole buffer to zero in case resizing to exact output dimension is not possible.
        // It's optional in case the image padding does not matter to be 0 value.
        // TODO: make padding and it's value an input option to this class
        memset(output_buff,0 , output_width* output_height*output_planes); 
    }

    //TODO : If the decoder info shows not a Jpeg image, try openCV to edcode


    if(_decoder->decode(input_buff,
                        size, 
                        output_buff, 
                        output_width, 
                        output_height, 
                        color_format) != Decoder::Status::OK) 
    {
        return LoaderModuleStatus::DECODE_FAILED;
    }

    return LoaderModuleStatus::OK;
}
