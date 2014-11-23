/**
 * @brief This file includes most of the code for the Audio System of the engine.
 * @file AudioSystem.h
 * @author Skyler Saleh, Ian Wiggins
 **/
#ifndef AudioTest_Audio_h
#define AudioTest_Audio_h
#include "Types.h"
#include <opusfile/opusfile.h>
#include <vorbis/vorbisfile.h>
#include "VirtualResource.h"
#include <map>
#include <vector>
#include <deque>
#include <math.h>
#include "Vector.h"
#include <iostream>
namespace Argon{

    enum{
        kAudioBufferSize=512,
        kAudioSampleRate=48000,
        kAudioNumberOfChannels=2,
        kAudioListenerBufferSamples = kAudioSampleRate/2
    };
    const float kAudioTimePerSample = 1.0/kAudioSampleRate;
    const float kMinVolume = 0.0001f;
    ///This is the base class of all the Audio effect/generators.
    struct AudioNode:virtual public ReflectionBase{
        /// These nodes form the root of the audio tree. They control what audio is outputted
        /// on each channel.
        static std::shared_ptr<AudioNode> root_nodes[kAudioNumberOfChannels];
        /// The volume of the node.
        float volume;
        AudioNode():volume(1.){}
        /// The input port of this AudioNode
        std::shared_ptr<AudioNode> input;
        /// Called to Render audio samples into the buffer, additively.
        virtual void render(float * buffer){}
        MAKE_VISIT(AudioNode,{ADD_VAR_DOUBLE(volume);})
    };
    /// A simple node that generates a sin wave.
    struct SinGenerator:public AudioNode{
        float phase;
        float frequency;
        SinGenerator():phase(0.0),frequency(1000){}
        SinGenerator(float frequency):phase(0.0),frequency(frequency){}

        virtual void render(float * buffer);
        MAKE_VISIT_HEAD(SinGenerator)
    };

    /// A simple node that generates a saw wave.
    struct SawGenerator:public AudioNode{
        float phase;
        float frequency;
        SawGenerator():phase(0.0),frequency(1000){}
        SawGenerator(float frequency):phase(0.0),frequency(frequency){}

        virtual void render(float * buffer);
        MAKE_VISIT_HEAD(SawGenerator)
    };

    /// A simple node that generates a triangle wave.
    struct TriangleGenerator:public AudioNode{
        float phase;
        float frequency;
        float direction;
        TriangleGenerator():phase(0.0),frequency(1000),direction(1){}
        TriangleGenerator(float frequency):phase(0.0),frequency(frequency),direction(1){}

        virtual void render(float * buffer);
        MAKE_VISIT_HEAD(TriangleGenerator)
    };
    /// A node that additively mixes an arbitrary amount of channels.
    struct Mixer : public AudioNode{

        std::vector<std::shared_ptr<AudioNode> > channels;
        virtual void render(float * buffer);
        MAKE_VISIT_HEAD(Mixer)
    };
    struct PeakDetector:public AudioNode{
        float peak=0;
        float decay_coeff= 0.06;
        float growth_coeff = 0.3;

        float average=0;
        virtual void render(float * buffer);

        MAKE_VISIT_HEAD(PeakDetector)


    };
    /// A node that duplicates the output of one AudioNode into another.
    class Repeater : public AudioNode {
        struct clone: public AudioNode{
            float * in_buffer;
            clone():in_buffer(NULL){}
            virtual void render(float * buffer);
        };
        float in_buff[kAudioBufferSize];
        std::map<size_t, std::shared_ptr<clone> > cloned_outputs;
        public:
        Repeater(){
            for(size_t i=0;i<kAudioBufferSize;++i)in_buff[i]=0;
        }

        virtual void render(float * buffer);
        std::shared_ptr<AudioNode> get_clone(size_t i);
    };
    /// A node that resamples its input.
    struct Resample: public AudioNode{
        float sample_buffer[kAudioBufferSize];
        float last;
        float rate;
        float sample_pos;
        float curr_pos;
        float low_pass =0;
        Resample():last(0),rate(1.),sample_pos(0.),curr_pos(0.){
            for(int i=0;i<kAudioBufferSize;++i)sample_buffer[i]=0;
        }
        float interopolate(float s1, float s2, float ratio){return s2*ratio+s1*(1.0-ratio);}
        virtual void render(float * buffer);
        MAKE_VISIT_HEAD(Resample)

    };

    /// An output Audio Node, that plays back from an opus file.
    struct OggNode{
        struct Clone: public AudioNode{
            OggNode* parent;
            std::deque<float> data;
            void render(float* buffer);
        };
        struct OggFileData{
            size_t offset;
            VirtualResource resource;
        };
        static int op_read(void *_stream,unsigned char *ptr,int nbytes){
            OggFileData* data = (OggFileData*)_stream;
            size_t read =data->resource.read((char*)ptr, nbytes, data->offset);
            data->offset+=read;
            return read;

        }
        static size_t ov_read(void *ptr,size_t bytes_per_comp,size_t comps,void *_stream){
            size_t nbytes=comps*bytes_per_comp;
            OggFileData* data = (OggFileData*)_stream;
            size_t read =data->resource.read((char*)ptr, nbytes, data->offset);
            comps = read/bytes_per_comp;
            data->offset+=comps*bytes_per_comp;
            return comps;

        }
        static int op_seek(void *stream,opus_int64 offset,int whence){
            OggFileData* data = (OggFileData*)stream;
            if(whence==SEEK_SET)data->offset=offset;
            else if(whence==SEEK_CUR)data->offset+=offset;
            else if(whence==SEEK_END)data->offset = data->resource.size()-offset;
            else return -1;

            return 0;
        }
        static int ov_seek(void *stream,opus_int64 offset,int whence){
            OggFileData* data = (OggFileData*)stream;
            if(whence==SEEK_SET)data->offset=offset;
            else if(whence==SEEK_CUR)data->offset+=offset;
            else if(whence==SEEK_END)data->offset = data->resource.size()-offset;
            else return -1;

            return data->offset;
        }
        static opus_int64 op_tell(void *_stream){
            return ((OggFileData*)_stream)->offset;
        }
        static long ov_tell(void *_stream){
            return ((OggFileData*)_stream)->offset;
        }
        static int op_close(void *_stream){
            delete (OggFileData*)_stream;
            return 0;
        }
        std::shared_ptr<Resample> channels[8];
        std::shared_ptr<Clone> clones[8];
        std::shared_ptr<Resample> left;
        std::shared_ptr<Resample>right;
        int error;
        bool initialized=false;
        OpusFileCallbacks opus_callbacks;
        ov_callbacks vorbis_callbacks;
        OggFileData file_data;
        OggOpusFile* opus_file;
        OggVorbis_File  vf;
        OggVorbis_File * vorbis_file;
        Argon::BoundedNumber<int,1,8> number_of_channels;
        float PCM_data[4096];
        bool loop;
        void init_opus(VirtualResource path){
            if(!path.get_source<VirtualResourceIMPL::Source*>()){
                std::cout <<"Could not fing OGG file: "<<path.get_path_string()<<std::endl;
                return;
            }
            playback_position=0;

            OggFileData * data = new OggFileData;
            data->offset =0;
            data->resource=path;
            opus_callbacks.read= &op_read;
            opus_callbacks.seek= &op_seek;
            opus_callbacks.tell= &op_tell;
            opus_callbacks.close = &op_close;
            opus_file=op_open_callbacks(data,&opus_callbacks,NULL,0,&error);
            vorbis_file =&vf;
            int vorbis_success=-1;
            if(!opus_file){
                data->offset=0;
                vorbis_callbacks.close_func=&op_close;
                vorbis_callbacks.seek_func=&ov_seek;
                vorbis_callbacks.tell_func=&ov_tell;
                vorbis_callbacks.read_func=&ov_read;

                vorbis_success=ov_open_callbacks(data,vorbis_file,NULL,0,vorbis_callbacks);
            }
            if(!opus_file&&vorbis_success<0){
                vorbis_file=NULL;
                std::cout<<"Failed to open ogg file: "<<path.get_path_string()<<"\n";
            }else initialized = true;

        }
        float playback_position;
        OggNode(){
            number_of_channels=2;
            for(int x=0;x<8;++x){
                channels[x]=std::make_shared<Resample>();
                clones[x]=std::make_shared<Clone>();
                channels[x]->input=clones[x];clones[x]->parent=this;
            }
            left=channels[0];
            right=channels[1];
            opus_file = NULL;
            loop =true;
            playback_position =0;
        }


        void read_data();
    };


    /// Represents a 3D audio source for use with a listener.
    struct AudioSource3D:public ReflectionBase{
        /// The root of the linked list of all 3D sources.
        static AudioSource3D *root_source;
        /// The next source in the list of all 3D sources.
        AudioSource3D * next_source;
        /// The previous source in the list of all 3D sources.
        AudioSource3D * prev_source;
        /// The position of the audio source.
        Vector3f position;
        /// The last position of the source, this is updated automatically.
        Vector3f last_position;
        Vector3f last_position2;

        /// Holds the audio data from its input.
        float audio_data[kAudioBufferSize];
        size_t last_frame;
        /// The input AudioNode for the source.
        std::shared_ptr<AudioNode> input;
        AudioSource3D();
        ~AudioSource3D();
        void update_if_needed(size_t current_frame);
        void finalize_data();
        MAKE_VISIT_HEAD(AudioSource3D)

    };

    /// An AudioNode that implements 3D Audio, by bipolar phase inteferance.
    struct Listener: public AudioNode{
        static size_t current_frame;
        /// The speed of sound in the units of the game.
        float speed_of_sound;
        /// The attenuation of air.
        float attenuation_of_air;
        /// Controls how directional the listener.
        float direction_factor;
        /// The position of the listener.
        Vector3f position=Vector3f(0,0,0);
        /// The last position of the listener, updated automatically.
        Vector3f last_position=Vector3f(0,0,0);
        /// The direction the listener is facing.
        Vector3f direction=Vector3f(0,0,0);
        size_t last_frame;
        float low_pass=0;

        float delay_buffer[kAudioListenerBufferSamples];
        RollingInt<0,kAudioListenerBufferSamples-1> delay_index;
        float intensity_at_distance_squared(float distance_squared){
            if(distance_squared<0.25)distance_squared=0.25;
            // http://www.sengpielaudio.com/calculator-soundpower.htm
            const float pi4 = 3.14159/4.;
            return 1.0f/(pi4*distance_squared+0.00001)*volume;
        }
        float intensity_at_distance(float distance){return intensity_at_distance_squared(distance*distance);}
        float sample_delay_for_distance(float dist){
            return float(kAudioSampleRate)/speed_of_sound*dist;
        }

        float interopolate(float s1, float s2, float ratio){
            return s2*ratio+s1*(1.0-ratio);
        }
        void update();
        void render(float * buffer);
        Listener();
        MAKE_VISIT_HEAD(Listener);


    };

};

#endif
