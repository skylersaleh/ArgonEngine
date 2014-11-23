/**
 * @brief Provides access to hardware.
 * @file Hardware.cpp
 * @author Skyler Saleh, Ian Wiggins
 **/

#include "Hardware.h"
#include "Vector.h"
#include <iostream>
#include <sstream>
#include <algorithm>
namespace Argon {


    Vector2f Screen::size(1,1);
    Vector2f Screen::position(1,1);
    Vector2f Screen::actual_size(1,1);
    Vector2f Screen::minimum_size(256,256);

    std::string Screen::title="Hello Argon!";
    bool Screen::full_screen = false;
    int      SleepLock::locks_ = 0;


    void SleepLock::prevent_sleep(){
        if(!have_lock_) locks_++;
        have_lock_ = true;
    }
    void SleepLock::allow_sleep(){
        if(have_lock_) locks_--;
        have_lock_ = false;
    }

    InputUpdate Input::get_last_update(double min_update, double max_time_ago){
        RollingInt<0, 511> ind = buffer_index()-1;
        RollingInt<0, 511> ind2 = ind+1;
        std::map<uint32_t, double> vals;
        std::map<uint32_t, double> prev;
        double current_time = timer().time();
        while(ind!=ind2){
            InputUpdate d = buffer()[ind];
            if(d.time+max_time_ago<current_time)break;
            uint32_t dev = d.input_id&kInputSourceMask;
            if(dev!=kInputOther&&dev!=kInputUnknown&&dev!=kInputWindow){
                InputUpdate d = buffer()[ind];

                double &v = prev[d.input_id];
                double &l = vals[d.input_id];
                l+=fabs(v-d.value);
                v=d.value;
                if(l>min_update)return buffer()[ind];
            }
            --ind;
        }
        InputUpdate d;
        d.input_id=0;
        d.time=0;
        d.value=0.;
        return d;
    }

    void Input::update_values(){
        while(value_index!=buffer_index()){
            InputUpdate &up=buffer()[value_index];
            uint32_t x = up.input_id;
            if(values.count(x)){
                InputState& s= values[x];

                double dt = up.time-s.last_time;

                s.magnitude*=std::max(1.0-s.magnitude_decay*dt,0.);
                s.magnitude+=std::abs(up.value-s.value);

                s.integral+=s.value*dt;

                float ratio = std::min(s.filter_ratio*dt,1.);
                s.filtered_value=s.value*ratio+s.filtered_value*(1.0-ratio);
                s.distance +=std::fabs(s.value-up.value);
                s.value=up.value;
                if(s.value<s.deadzone&&s.value>s.deadzone*-1.)s.value=0.;

                s.last_time=up.time;
            }
            ++value_index;
        }
        double current = timer().time();
        std::map<uint32_t, InputState>::iterator it = values.begin();
        while(it!=values.end()){
            InputState& s= it->second;
            double dt = current-s.last_time;

            s.magnitude*=std::max(1.0-s.magnitude_decay*dt,0.);

            s.integral+=s.value*dt;

            double ratio = std::min(s.filter_ratio*dt,1.);
            s.filtered_value=s.value*ratio+s.filtered_value*(1.0-ratio);
            s.last_time=current;
            ++it;
        }
    }
    void Input::push_update(uint32_t input_id,double value){
        size_t i = buffer_index();
        buffer()[i].input_id=input_id;
        buffer()[i].value=value;
        buffer()[i].time =timer().time();
        buffer_index()++;
    }
    const char* Input::get_source_string(uint32_t input_id){
        switch (input_id&kInputSourceMask) {
            case kInputKeyboard:return "Key";
            case kInputMouse:   return "Mouse";
            case kInputJoy:     return "Joystick";
            case kInputTouch:   return "Touchscreen";
            case kInputAccelerometer: return "Accelerometer";
            case kInputGyroscope:return "Gyroscope";
            case kInputCompass:  return "Compass";
            case kInputWindow:   return "Window";
        case kInputOther: return "Other";
            case kInputVirtual:  return "Virtual";
        }
        return "Unknown";
    }
    std::string Input::get_mouse_button_string(uint32_t input_id){
        if (input_id==kInputIDMouseX) return "X Axis";
        if (input_id==kInputIDMouseY) return "Y Axis";
        if (input_id==kInputIDMouseHorzScroll) return "Horz Scroll";
        if (input_id==kInputIDMouseVertScroll) return "Vert Scroll";
        if (input_id==kInputIDMouseLeft) return "Left Click";
        if (input_id==kInputIDMouseRight) return "Right Click";
        if (input_id==kInputIDMouseMiddle) return "Middle Click";
        std::stringstream s;
        s<<"Button "<<input_id-kInputIDMouseButton0;
        return s.str();


    }
    std::string Input::get_generic_axis_string(uint32_t input_id){
        switch (input_id&kInputAxisMask) {
            case kInputAxisX:return "X Axis";
            case kInputAxisY:return "Y Axis";
            case kInputAxisZ:return "Z Axis";
        case kInputAxisX2:return "X2 Axis";
        case kInputAxisY2:return "Y2 Axis";
        case kInputAxisZ2:return "Z2 Axis";
        }
        std::stringstream s;
        if(input_id&kInputAxisAnalog)s<<"Axis ";
        else s<<"Button ";

        s<<(input_id&kInputButtonMask);
        return s.str();
    }

    std::string Input::get_key_string(uint32_t input_id){
        switch (input_id)
        {
                // Printable keys

                // Function keys
            case kInputIDEscape:      return "Escape";
            case kInputIDF1:          return "F1";
            case kInputIDF2:          return "F2";
            case kInputIDF3:          return "F3";
            case kInputIDF4:          return "F4";
            case kInputIDF5:          return "F5";
            case kInputIDF6:          return "F6";
            case kInputIDF7:          return "F7";
            case kInputIDF8:          return "F8";
            case kInputIDF9:          return "F9";
            case kInputIDF10:         return "F10";
            case kInputIDF11:         return "F11";
            case kInputIDF12:         return "F12";
            case kInputIDF13:         return "F13";
            case kInputIDF14:         return "F14";
            case kInputIDF15:         return "F15";
            case kInputIDF16:         return "F16";
            case kInputIDF17:         return "F17";
            case kInputIDF18:         return "F18";
            case kInputIDF19:         return "F19";
            case kInputIDF20:         return "F20";
            case kInputIDF21:         return "F21";
            case kInputIDF22:         return "F22";
            case kInputIDF23:         return "F23";
            case kInputIDF24:         return "F24";
            case kInputIDF25:         return "F25";
            case kInputIDUp:          return "Up";
            case kInputIDDown:        return "Down";
            case kInputIDLeft:        return "Left";
            case kInputIDRight:       return "Right";
            case kInputIDLeftShift:   return "Left Shift";
            case kInputIDRightShift:  return "Right Shift";
            case kInputIDLeftControl: return "Left Control";
            case kInputIDRightControl:return "Right Control";
            case kInputIDLeftAlt:     return "Left ALT";
            case kInputIDRightAlt:    return "Right ALT";
            case kInputIDTab:         return "Tab";
            case kInputIDEnter:       return "Enter";
            case kInputIDBackspace:   return "Backspace";
            case kInputIDInsert:      return "Insert";
            case kInputIDDelete:      return "Delete";
            case kInputIDPageUp:      return "Page Up";
            case kInputIDPageDown:    return "Page Down";
            case kInputIDHome:        return "Home";
            case kInputIDEnd:         return "End";
            case kInputIDKP0:         return "Keypad 0";
            case kInputIDKP1:         return "Keypad 1";
            case kInputIDKP2:         return "Keypad 2";
            case kInputIDKP3:         return "Keypad 3";
            case kInputIDKP4:         return "Keypad 4";
            case kInputIDKP5:         return "Keypad 5";
            case kInputIDKP6:         return "Keypad 6";
            case kInputIDKP7:         return "Keypad 7";
            case kInputIDKP8:         return "Keypad 8";
            case kInputIDKP9:         return "Keypad 9";
            case kInputIDKPDivide:    return "Keypad Divide";
            case kInputIDKPMultiply:  return "Keypad Multiply";
            case kInputIDKPSubtract:  return "Keypad Subtract";
            case kInputIDKPAdd:       return "Keypad Add";
            case kInputIDKPDecimal:   return "Keypad Decimal";
            case kInputIDKPEqual:     return "Keypad Equal";
            case kInputIDKPEnter:     return "Keypad Enter";
            case kInputIDPrintScreen: return "Print Screen";
            case kInputIDNumLock:     return "NUM Lock";
            case kInputIDCapsLock:    return "CAPS Lock";
            case kInputIDScrollLock:  return "Scroll Lock";
            case kInputIDPause:       return "Pause";
            case kInputIDLeftSuper:   return "Left Super";
            case kInputIDRightSuper:  return "Right Super";
            case kInputIDMenu:        return "Menu";

            default:                   return std::string()+char(input_id);
        }

    }
    std::string Input::get_window_string(uint32_t input_id){
        switch (input_id) {
            case kInputIDWindowInactive:return "Inactive";
            case kInputIDWindowHasMouse:return "Has Mouse";
            case kInputIDWindowMinimized:return "Minimized";
        }
        std::stringstream s;
        s<<(input_id&kInputAxisMask);
        return s.str();
    }

    std::string Input::get_full_input_string(uint32_t input_id){
        std::stringstream str;
        str<<get_source_string(input_id)<<" ";
        switch (input_id&kInputSourceMask) {
            case kInputKeyboard:
                str<<get_key_string(input_id);
                break;
            case kInputMouse:
                str<<get_mouse_button_string(input_id);
                break;
            case kInputTouch:
            case kInputJoy: str<<get_device(input_id)<<" ";
            case kInputCompass:
            case kInputAccelerometer:
            case kInputVirtual:
            case kInputGyroscope:
                str<<get_generic_axis_string(input_id);
                break;
            case kInputWindow:
                str<<get_window_string(input_id);
                break;
            default:
                str<<input_id;
                break;
        }
        return str.str();
    }
    MAKE_VISIT_IMPL(InputState, {
        ADD_VAR_DOUBLE(value);
        ADD_VAR_DOUBLE(magnitude);
        ADD_VAR_DOUBLE(magnitude_decay);
        ADD_VAR_DOUBLE(distance);
        ADD_VAR_DOUBLE(integral);
        ADD_VAR_DOUBLE(filtered_value);
        ADD_VAR_DOUBLE(filter_ratio);
        ADD_VAR_DOUBLE(last_time);
        ADD_VAR_DOUBLE(offset);
    });
    MAKE_VISIT_IMPL(InputUpdate, {
        ADD_VAR_LONG(input_id);
        ADD_VAR_DOUBLE(value);
        ADD_VAR_DOUBLE(time);
    });
};
