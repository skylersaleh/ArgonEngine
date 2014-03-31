//Generated by the Argon Build System


/**
 * @file AdController.h
 * @author Skyler Saleh
 * @date   June 15, 2013
 *
 * @brief This file implemements an abstract interface for displaying in-app advertisements.
 * Ads will be shown when there is any instances of this class that are not hidden.
 **/

#ifndef ArgonEngine_AdController_h
#define ArgonEngine_AdController_h
#include "Thread.h"
#include "Utility.h"
namespace Argon {
class AdController {
    static int banner_users;
    static bool show_pop_up_;
    static bool show_cross_app_promotion_;
    static StringIntern pop_up_id;
    bool banner_visible;
    static Mutex& mutex(){
        static Mutex m; return m;
    }
public:
    AdController(bool b) : banner_visible(false){
        if(b) show_banner();
    }
    AdController() : banner_visible(false){
        show_banner();
    }
    void hide_banner();     //!< Makes this instance hidden
    void show_banner();     //!< Makes the instance visible
    static StringIntern get_pop_up_id(){
        return pop_up_id;
    }                                                           //!< Returns the identifier of the popup
    static void show_pop_up(){
        show_pop_up("Untitled");
    }                                                           //!< Shows a pop-up
    static void show_pop_up(const StringIntern& ident);         //!< Shows a pop-up and gives it an id

    static bool should_show_pop_up();                           //!< Returns true if a pop-up should be shown
    static bool should_show_cross_app_promotion();              //!< Returns true if a cross app promotion page should be shown
    static void show_cross_app_promotion();                     //!< Shows the cross app promotion page
    static bool should_show_banner(){
        return banner_users;
    }                                                           //!< returns true if ads should be shown
    ~AdController(){
        hide_banner();
    }
};

};

#endif // ifndef ArgonEngine_AdController_h
