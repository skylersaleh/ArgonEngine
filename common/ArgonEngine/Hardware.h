/**
 * @brief Provides a means to access various hardware of the device.
 * @file Hardware.h
 * @author Skyler Saleh, Ian Wiggins
 **/

#ifndef NeonRush_ARHardware_h
#define NeonRush_ARHardware_h
#include "Types.h"
#include "Vector.h"
#include <map>
#include <ArgonEngine/Thread.h>
namespace Argon {
    ///Used to control the sleep behaviour of the device
    class SleepLock {
    private:
        static int locks_;
        bool have_lock_;
    public:
        SleepLock() : have_lock_(false){
            prevent_sleep();
        }
        void prevent_sleep();    //stops device from sleeping
        void allow_sleep();      //allows the device to sleep
        static bool sleep_enabled(){
            return !locks_;
        }
        ~SleepLock(){
            allow_sleep();
        }
    };

    /// This class contains the static properties of the screen/window being rendered.
    struct Screen {
        static Vector2f size; /// Size of the virtual screen in pixels.
        static Vector2f actual_size; /// The actual hardware screen size.
        static Vector2f minimum_size;
        static Vector2f position;
        /// The ratio of height/width of the screen.
        static bool full_screen;
        static std::string title;
        static float ratio(){
            return size[1]/size[0];
        }
        static float actual_ratio(){
            return actual_size[1]/actual_size[0];
        }
    };


    enum InputSource {
        kInputKeyboard  =0x00000000,
        kInputMouse     =0x01000000,
        kInputJoy       =0x02000000,
        kInputTouch     =0x03000000,

        kInputAccelerometer =0x04000000,

        kInputGyroscope =0x05000000,
        kInputCompass   =0x06000000,

        kInputWindow    =0x07000000,
        kInputOther     =0x08000000,
        kInputUnknown   =0x09000000,

        kInputVirtual   =0x10000000,

        kInputSourceLast=0xFF000000,
        kInputSourceMask      =0xFF000000,
        kInputSourceIncrement =0x01000000,
        kInputSourceShift = 24

    };
    enum InputDevice {
        kInputDevice0 = 0x000000,
        kInputDevice1 = 0x010000,
        kInputDevice2 = 0x020000,
        kInputDevice3 = 0x030000,
        kInputDevice4 = 0x040000,
        kInputDevice5 = 0x050000,
        kInputDevice6 = 0x060000,
        kInputDevice7 = 0x070000,

        kInputDeviceLast = 0xFF0000,
        kInputDeviceMask = 0xFF0000,

        kInputDeviceIncrement = 0x010000,
        kInputDeviceShift =16

    };
    enum InputAxis {
        kInputAxisAnalog=0x8000,

        kInputAxisButtonStart=0,
        kInputAxisAnalogStart=kInputAxisAnalog,
        kInputAxisHatStart=kInputAxisAnalog+128,

        kInputAxisX=kInputAxisAnalog+0,
        kInputAxisY=kInputAxisAnalog+1,
        kInputAxisZ=kInputAxisAnalog+2,

        kInputAxisX2=kInputAxisAnalog+3,
        kInputAxisY2=kInputAxisAnalog+4,
        kInputAxisZ2=kInputAxisAnalog+5,

        kInputHatX = kInputAxisAnalog+128,
        kInputHatY = kInputAxisAnalog+129,
        kInputHatZ = kInputAxisAnalog+130,

        kInputHatX2 = kInputAxisAnalog+131,
        kInputHatY2 = kInputAxisAnalog+132,
        kInputHatZ2 = kInputAxisAnalog+133,

        kInputAxisLast=0xFFFF,
        kInputAxisMask=0xFFFF,
        kInputButtonLast=0x7FFF,
        kInputButtonMask=0x7FFF,
        kInputAxisIncrement=1,
        kInputAxisShift =0
    };
    enum BatteryState{
        kBatteryCharging,
        kBatteryDischarging,
        kBatteryFull,
        kNoBattery
    };
    enum InputIDs {
        kInputIDMouseX=kInputMouse|kInputAxisX,
        kInputIDMouseY=kInputMouse|kInputAxisY,
        kInputIDMouseHorzScroll=kInputMouse|kInputAxisAnalog|3,
        kInputIDMouseVertScroll=kInputMouse|kInputAxisAnalog|4,

        kInputIDMouseLeft=kInputMouse|1,
        kInputIDMouseMiddle=kInputMouse|2,
        kInputIDMouseRight=kInputMouse|3,

        kInputIDMouseButton0=kInputMouse|0,
        kInputIDMouseButton1=kInputMouse|1,
        kInputIDMouseButton2=kInputMouse|2,
        kInputIDMouseButton3=kInputMouse|3,
        kInputIDMouseButton4=kInputMouse|4,
        kInputIDMouseButton5=kInputMouse|5,
        kInputIDMouseButton6=kInputMouse|6,
        kInputIDMouseButton7=kInputMouse|7,
        kInputIDMouseButton8=kInputMouse|8,

        kInputIDAccelX =kInputAccelerometer|kInputAxisX,
        kInputIDAccelY =kInputAccelerometer|kInputAxisY,
        kInputIDAccelZ =kInputAccelerometer|kInputAxisZ,

        kInputIDGyroX =kInputGyroscope|kInputAxisX,
        kInputIDGyroY =kInputGyroscope|kInputAxisY,
        kInputIDGyroZ =kInputGyroscope|kInputAxisZ,

        kInputIDCompassX =kInputCompass|kInputAxisX,
        kInputIDCompassY =kInputCompass|kInputAxisY,
        kInputIDCompassZ =kInputCompass|kInputAxisZ,

        kInputIDTouch0X =kInputTouch|kInputDevice0|kInputAxisX,
        kInputIDTouch0Y =kInputTouch|kInputDevice0|kInputAxisY,
        kInputIDTouch0Z =kInputTouch|kInputDevice0|kInputAxisZ,

        kInputIDTouch1X =kInputTouch|kInputDevice1|kInputAxisX,
        kInputIDTouch1Y =kInputTouch|kInputDevice1|kInputAxisY,
        kInputIDTouch1Z =kInputTouch|kInputDevice1|kInputAxisZ,

        kInputIDTouch2X =kInputTouch|kInputDevice2|kInputAxisX,
        kInputIDTouch2Y =kInputTouch|kInputDevice2|kInputAxisY,
        kInputIDTouch2Z =kInputTouch|kInputDevice2|kInputAxisZ,

        kInputIDTouch3X =kInputTouch|kInputDevice3|kInputAxisX,
        kInputIDTouch3Y =kInputTouch|kInputDevice3|kInputAxisY,
        kInputIDTouch3Z =kInputTouch|kInputDevice3|kInputAxisZ,

        kInputIDTouch4X =kInputTouch|kInputDevice4|kInputAxisX,
        kInputIDTouch4Y =kInputTouch|kInputDevice4|kInputAxisY,
        kInputIDTouch4Z =kInputTouch|kInputDevice4|kInputAxisZ,

        kInputIDTouch5X =kInputTouch|kInputDevice5|kInputAxisX,
        kInputIDTouch5Y =kInputTouch|kInputDevice5|kInputAxisY,
        kInputIDTouch5Z =kInputTouch|kInputDevice5|kInputAxisZ,

        kInputIDTouch6X =kInputTouch|kInputDevice6|kInputAxisX,
        kInputIDTouch6Y =kInputTouch|kInputDevice6|kInputAxisY,
        kInputIDTouch6Z =kInputTouch|kInputDevice6|kInputAxisZ,

        kInputIDTouch7X =kInputTouch|kInputDevice7|kInputAxisX,
        kInputIDTouch7Y =kInputTouch|kInputDevice7|kInputAxisY,
        kInputIDTouch7Z =kInputTouch|kInputDevice7|kInputAxisZ,
        kInputIDEscape    = kInputKeyboard|256,
        kInputIDEnter     = kInputKeyboard|257,
        kInputIDTab       = kInputKeyboard|258,
        kInputIDBackspace = kInputKeyboard|259,
        kInputIDInsert    = kInputKeyboard|260,
        kInputIDDelete    = kInputKeyboard|261,
        kInputIDRight     = kInputKeyboard|262,
        kInputIDLeft      = kInputKeyboard|263,
        kInputIDDown      = kInputKeyboard|264,
        kInputIDUp        = kInputKeyboard|265,

        kInputIDPageUp    = kInputKeyboard|266,
        kInputIDPageDown  = kInputKeyboard|267,
        kInputIDHome      = kInputKeyboard|268,
        kInputIDEnd       = kInputKeyboard|269,
        kInputIDCapsLock  = kInputKeyboard|280,
        kInputIDScrollLock= kInputKeyboard|281,
        kInputIDNumLock   = kInputKeyboard|282,
        kInputIDPrintScreen= kInputKeyboard|283,
        kInputIDPause     = kInputKeyboard|284,

        kInputIDF1        = kInputKeyboard|290,
        kInputIDF2        = kInputKeyboard|291,
        kInputIDF3        = kInputKeyboard|292,
        kInputIDF4        = kInputKeyboard|293,
        kInputIDF5        = kInputKeyboard|294,
        kInputIDF6        = kInputKeyboard|295,
        kInputIDF7        = kInputKeyboard|296,
        kInputIDF8        = kInputKeyboard|297,
        kInputIDF9        = kInputKeyboard|298,
        kInputIDF10       = kInputKeyboard|299,

        kInputIDF11        = kInputKeyboard|300,
        kInputIDF12        = kInputKeyboard|301,
        kInputIDF13        = kInputKeyboard|302,
        kInputIDF14        = kInputKeyboard|303,
        kInputIDF15        = kInputKeyboard|304,
        kInputIDF16        = kInputKeyboard|305,
        kInputIDF17        = kInputKeyboard|306,
        kInputIDF18        = kInputKeyboard|307,
        kInputIDF19        = kInputKeyboard|308,
        kInputIDF20        = kInputKeyboard|309,

        kInputIDF21        = kInputKeyboard|310,
        kInputIDF22        = kInputKeyboard|311,
        kInputIDF23        = kInputKeyboard|312,
        kInputIDF24        = kInputKeyboard|313,
        kInputIDF25        = kInputKeyboard|314,

        kInputIDKP0        = kInputKeyboard|320,
        kInputIDKP1        = kInputKeyboard|321,
        kInputIDKP2        = kInputKeyboard|322,
        kInputIDKP3        = kInputKeyboard|323,
        kInputIDKP4        = kInputKeyboard|324,
        kInputIDKP5        = kInputKeyboard|325,
        kInputIDKP6        = kInputKeyboard|326,
        kInputIDKP7        = kInputKeyboard|327,
        kInputIDKP8        = kInputKeyboard|328,
        kInputIDKP9        = kInputKeyboard|329,

        kInputIDKPDecimal  = kInputKeyboard|330,
        kInputIDKPDivide   = kInputKeyboard|331,
        kInputIDKPMultiply = kInputKeyboard|332,
        kInputIDKPSubtract = kInputKeyboard|333,
        kInputIDKPAdd      = kInputKeyboard|334,
        kInputIDKPEnter    = kInputKeyboard|335,
        kInputIDKPEqual    = kInputKeyboard|336,

        kInputIDLeftShift  = kInputKeyboard|340,
        kInputIDLeftControl= kInputKeyboard|341,
        kInputIDLeftAlt    = kInputKeyboard|342,
        kInputIDLeftSuper  = kInputKeyboard|343,

        kInputIDRightShift  = kInputKeyboard|344,
        kInputIDRightControl= kInputKeyboard|345,
        kInputIDRightAlt    = kInputKeyboard|346,
        kInputIDRightSuper  = kInputKeyboard|347,
        kInputIDMenu        = kInputKeyboard|348,

        kInputIDWindowInactive = kInputWindow|1,
        kInputIDWindowHasMouse = kInputWindow|2,
        kInputIDWindowMinimized= kInputWindow|3,

        kInputIDBatteryTimeRemaining = kInputOther|1,
        kInputIDBatteryPercent = kInputOther|2,
        kInputIDBatteryState = kInputOther|4,

        kInputIDJoyX = kInputJoy|kInputAxisX,
        kInputIDJoyY = kInputJoy|kInputAxisY,
        kInputIDJoyZ = kInputJoy|kInputAxisZ,

        kInputIDJoyX2 = kInputJoy|kInputAxisX2,
        kInputIDJoyY2 = kInputJoy|kInputAxisY2,
        kInputIDJoyZ2 = kInputJoy|kInputAxisZ2,

        kInputIDJoyHatX = kInputJoy|kInputHatX,
        kInputIDJoyHatY = kInputJoy|kInputHatY,
        kInputIDJoyHatZ = kInputJoy|kInputHatZ,

        kInputIDJoyAccelX = kInputJoy|kInputHatX,
        kInputIDJoyAccelY = kInputJoy|kInputHatY,
        kInputIDJoyAccelZ = kInputJoy|kInputHatZ,


        kInputIDJoyHatX2 = kInputJoy|kInputHatX2,
        kInputIDJoyHatY2 = kInputJoy|kInputHatY2,
        kInputIDJoyHatZ2 = kInputJoy|kInputHatZ2,

        kInputIDJoyButton0 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*0),
        kInputIDJoyButton1 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*1),
        kInputIDJoyButton2 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*2),
        kInputIDJoyButton3 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*3),
        kInputIDJoyButton4 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*4),
        kInputIDJoyButton5 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*5),
        kInputIDJoyButton6 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*6),
        kInputIDJoyButton7 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*7),
        kInputIDJoyButton8 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*8),
        kInputIDJoyButton9 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*9),
        kInputIDJoyButton10 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*10),
        kInputIDJoyButton11 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*11),
        kInputIDJoyButton12 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*12),
        kInputIDJoyButton13 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*13),
        kInputIDJoyButton14 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*14),
        kInputIDJoyButton15 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*15),
        kInputIDJoyButton16 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*16),
        kInputIDJoyButton17 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*17),
        kInputIDJoyButton18 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*18),
        kInputIDJoyButton19 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*19),
        kInputIDJoyButton20 = kInputJoy|(kInputAxisButtonStart+kInputAxisIncrement*20),






    };
    struct InputUpdate :public ReflectionBase{
        uint32_t input_id=kInputUnknown;///{16bits Axis, 8bits device number, 8bits source type}
        double value;     ///The new value
        double time;      ///The timestamp of the last update
        MAKE_VISIT_HEAD(InputUpdate)
    };

    ///The InputState struct contains useful data and derived data for an input source.
    struct InputState :public ReflectionBase{
        /// The current value of the input.
        double value=0;
        /// The current magnitude of the inputs motion.
        /// This is useful when you want to know things like how fast the user is pressing a button, or moving a joystick
        /// back and forth.
        double magnitude=0;
        /// How quickly the magnitude decays between 1 and 0, the closer it is to 0 the faster it decays.
        double magnitude_decay=0.9;
        /// The integral of the input. Useful for things like finding out how long a button has been pressed.
        double deadzone=0.0;
        double integral=0;
        /// The low-pass filtered value of the input.
        double filtered_value=0;
        /// The coefficient for the IIR low-pass filter, between 1 and 0.
        double filter_ratio=0.95;
        /// The timestamp of the last update.
        double last_time=0;
        /// An offset that is added to the actual value before all other variables are computed.
        /// This is useful for providing calibration features.
        double offset=0;
        /// The length that the input has traveled.
        double distance =0;
        operator bool(){return value>=0.5;}
        MAKE_VISIT_HEAD(InputState);

    };

    /**
     * @breif The Input class allows you to get data from various input devices such as mice, keyboards, joysticks,
     * etc., and various information from the HAL.
     *
     * @details Each Input object spawned, and all of its input states are completely seperate from each other.
     * This allows you to make modifications to the InputState of an input device, and calculate things like the magnitude,
     * without effecting the values of the same input read using a different Input object. Which helps keep the code very modular.
     **/
    struct Input {
        /// The global input ring-buffer. If the buffer overflows between input updates the state changes are lost.
        static InputUpdate* buffer(){
            static InputUpdate i[512]; return i;
        }
        /// The current global buffer index.
        static RollingInt<0, 511>& buffer_index(){
            static RollingInt<0, 511> x=0; return x;
        }
        /// The timer used for input timestamping.
        static Timer & timer(){
            static Timer t; return t;
        }
        RollingInt<0, 511> value_index; /// The index of this input object.
        RollingInt<0, 511> read_index;  /// The read index of this input object.

        /// A map of all active input states.
        std::map<uint32_t, InputState> values;

        Input() : value_index(0),read_index(0){
            value_index=read_index=buffer_index();
        }
        InputUpdate get_last_update(double min_update=0.5,double max_time_ago=3.);

        /// Updates the derived values for all observed input devices of this object.
        void update_values();
        /// Push a value update for a certain id. By using this with virtual inputs, you can abstract the
        /// control scheme from the game logic.
        static void push_update(uint32_t input_id,double value);
        /// Get a string that represents the name of a integer input id.
        static const char* get_source_string(uint32_t input_id);
        static std::string get_key_string(uint32_t input_id);
        static std::string get_full_input_string(uint32_t input_id);
        static std::string get_mouse_button_string(uint32_t input_id);
        static std::string get_window_string(uint32_t input_id);

        static std::string get_generic_axis_string(uint32_t input_id);
        /// Print the contents of the global input buffer, for debug purposes.
        template<typename T> void print_buffer(T& stream){
            RollingInt<0, 511> x= value_index;
            while (x!=buffer_index()) {
                stream<<get_full_input_string(buffer()[x].input_id)<< " = " <<buffer()[x].value<<"\n";
                ++x;
            }
        }
        /// Extract the device from an input_id
        static int get_device(uint32_t input_id){
            return (input_id&kInputDeviceMask)>>16;
        }
        /// Extract the source from an input_id
        static int get_source(uint32_t input_id){
            return (input_id&kInputSourceMask)>>24;
        }
        /// Fetch an InputState without updating it.
        InputState& fetch_no_update(uint32_t input_id){
            return values[input_id];
        }
        /// Fetch an InputState, and update it if needed.
        InputState& operator[](uint32_t input_id){

            uint32_t inp =input_id;
#ifdef USE_GLFW
            if(inp>96&&inp<123){
                inp-=32;
            }
#endif
#ifdef USE_SDL
            if(inp>=65&&inp<=90){
                inp+=32;
            }
#endif
            if(values[inp].last_time==0.&&(input_id& kInputSourceMask)==kInputJoy)
                values[inp].deadzone=0.1;
            update_values(); return values[inp];
        }

    };

};

#endif
