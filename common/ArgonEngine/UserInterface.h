/**
 * @brief This file contains the base classes for the User Interface Library
 * @file UserInterface.h
 * @author Skyler Saleh
 **/

#ifndef UserInterface_h
#define UserInterface_h
#include "Vector.h"
#include "BoundingCube.h"
#include "Utility.h"
#include "Thread.h"
#include "TypeInfo.h"
namespace Argon {
/*
class UIObject;
//! @brief Represents a single point of user interaction. Such as a mouse cursor, selection box, or touch.
class UIInteractionPoint :public InstanceList<UIInteractionPoint>{
    friend struct TypeInfo<UIInteractionPoint>;
private:
    UIObject * exclusive;
    Vector2f position;
    bool can_become_exclusive;
    ObjectTracker<UIInteractionPoint> tracker;
public:
    //! Creates a UIInteraction point at location 'pos' in Screen Coords.
    explicit UIInteractionPoint(Vector2f pos) : exclusive(NULL),position(pos),can_become_exclusive(false){
        tracker.register_tracker(this);
    }
    //! Copy Constructor
    UIInteractionPoint(const UIInteractionPoint& p) : exclusive(p.exclusive),position(p.position),can_become_exclusive(p.can_become_exclusive){
        tracker.register_tracker(this);
    }
    //! Returns the UIObject that the Interaction Point is exclusive to. This returns NULL if there is no exclusive object.
    UIObject* get_exclusive(){
        return exclusive;
    }
    //! Sets the position of the interaction point in Screen coordinates.
    void set_position(Vector2f pos){
        position=pos;
        tracker.set_modified();
    }
    //! Ends the interaction with a UIObject. This should be called when an interaction has ended. Ex. User releases from a click, stops touching screen, etc.
    void end_exclusive();
    //! Marks the start of a user interaction. Calling this function will shoot a ray through the scene starting at the position of the interaction point. It will then
    //! make the UIInteraction point exclusive to the front-most UIObject that the ray hits. If then makes the UIObject active if possible.
    //! If the ray does not hit anything. Exclusive is set to NULL, and any existing interaction from the point is canceled.
    //! An example of when you would want to call this function when a user first clicks or touches something.
    void set_exclusive(){
        can_become_exclusive=true;
        tracker.set_modified();
        can_become_exclusive=false;
    }
    //! Same as set_exclusive() except you force a certain object to be exclusive in this case. Will fail and return false if the point already
    //! has an object exclusive to it.
    bool set_exclusive_object(UIObject* object){
        if(can_become_exclusive&&!exclusive) {
            exclusive=object;
            return true;
        }
        return object==exclusive;
    }
    //! Returns the current position of the Interaction point.
    Vector2f get_position() const {
        return position;
    }
    virtual ~UIInteractionPoint(){
        end_exclusive();
    }
};
MAKE_TYPE_INFO_Argon(UIInteractionPoint, {ADD_VAR(position); ADD_VAR(can_become_exclusive); });
//@brief A UIObject represents an user interface element such as a button.
class UIObject : public ObjectTracer<UIInteractionPoint>{
private:
    static UIObject* active;
    OrderedVector<UIInteractionPoint*> hovering;
public:
    bool enabled;      //!< True if the UIObject interact able, false if it is disabled.
    //! This function is called when a UIObject first becomes active. When this happens, the function should set active to this;
    virtual void become_active(){
        active=this;
    }
    UIObject(){
        enabled=true; start_tracing();
    }
    Vector3f anchor;               //!< The center point of the UIObject. This is used for navigation by relative devices, such as a joystick.
    BoundingCube bounds;           //!< The bounds of the UIObject. This is used as a Broad Phase Collision detection for a raycast by a UIInteractionPoint.

    //! This is called at the start of an interaction. It should return true if it can make a UIInteractionPoint exclusive to it.
    virtual bool start_exclusive(UIInteractionPoint* point){
        return true;
    }
    //! This is called at the end of an interaction. It should make the object active if the UIInteractionPoint is still within its bounds.
    virtual void end_exclusive(UIInteractionPoint* p){
        if(intersects(p->get_position())) become_active();
    }
    //! This function is for raycasting a UIInteractionPoint. The rays direction is assumed to be directly down into the screen.
    //! Overload this if you need better accuracy than a bounding box can offer.
    //! Returns true if the ray intersects the object.
    virtual bool intersects(const Vector2f &point){
        return bounds.overlaps_point(point);
    }
    //! This gets called whenever a new UIInteraction point is created.
    virtual void handle_new_object(UIInteractionPoint*p){
        if(intersects(p->get_position())&&enabled)
        {
            if(p->set_exclusive_object(this)) hovering.insert(p);
        }else hovering.erase(p);
    }
    //! This gets called when an update occurs to UIInteractionPoint p
    virtual void handle_updated_object(UIInteractionPoint*p){
        if(intersects(p->get_position())&&enabled)
        {
            if(p->set_exclusive_object(this)) hovering.insert(p);
        }else hovering.erase(p);
    }
    //! This gets called before the interaction point p is deleted.
    virtual void handle_removed_object(UIInteractionPoint*p){
        hovering.erase(p);
    }
    //! Returns true if object is the active one.
    bool is_active(){
        return active==this;
    }
    //! Returns true if their are interaction points hovering above the object.
    bool is_hovering(){
        return hovering.size() !=0;
    }
    //! Called to end this objects stint as the active one.
    void end_active(){
        if(is_active()) active=NULL;
    }
};
MAKE_TYPE_INFO_Argon(UIObject, {
                         ADD_VAR(enabled);
                         ADD_VAR(anchor);
                         ADD_VAR(bounds);

                     });
//! @brief The base UIObject class as is already provides all needed functionality of a button. This is just for clarity and
//! to reserve the name in case it is needed later.
struct UIButton : public UIObject {};
MAKE_TYPE_INFO_Argon(UIButton, {
                         ADD_BASE(UIObject);
                         ADD_VAR(enabled);
                         ADD_VAR(anchor);
                         ADD_VAR(bounds);

                     });
*/

/**
 * @brief Manages events that happen external to your application. That interrupts the user from its standard flow control.
 * For instance the mouse leaving the application screen. The user minimizing your application, or when another program
 * steals focus from yours.
 **/
 
class UIInterruption  {
private:
    static double last_tick_time;
    static Timer timer;
public:
    static double interruption_timeout;     //!< The amount of time in seconds before an interruption becomes a persistent one. Defaults to 15 seconds.
    UIInterruption(){}
    //! Returns true if the app is currently being interrupted.
    //! During an interruption Audio Processing is halted, and it is a good idea to display a pause screen.
    //! Also the FPS may be reduced. One cause for this is if another app steals focus from yours or the users cursor leaves the screen.
    bool is_interrupted(){
        return last_tick_time+1.0<=timer.time();
    }

    //! Call this to advance the last_tick_timer. This needs to be called once a second (Normally by the HAL) to keep an interruption from happening.
    static void tick(){
        last_tick_time=timer.time();
    }

    //! Immediately register an interruption.
    static void set_interrupted(){
        last_tick_time=timer.time()-1.0;
    }
    //! Immediately register a persistent interruption.
    static void set_persistent_interrupt(){
        last_tick_time=-1e15;
    }
    //! Returns true if a persistent interruption is currently happening. This can for instance, when a user minimizes your app.
    //! When this happens you should return from your animate functions as quickly as possible, to conserve processing for the user.
    //! The render loop is halted, and the screen stops updating.
    //! Also it is a good idea to begin saving data, as this can indicate the user is about to quit the application.
    static bool is_persistent_interruption(){
        return last_tick_time+interruption_timeout<timer.time();
    }
};

};

#endif
