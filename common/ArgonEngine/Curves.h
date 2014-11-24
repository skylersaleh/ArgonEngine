/**
 * @brief Some basic curves functionality. Only an example for now.
 * @file Curves.h
 * @author Ian Wiggins
 **/

#ifndef CURVES_H
#define CURVES_H

#include <list>
#include <map>

///Usage:
/// Create a simple curve based off of a goal.
/// An example of using this would be in a node, give it a pointer to a sprite.color, and a goal color.
/// It would then smoothly change the sprite's color to the goal.
///
/// If BGEnable is enabled, then once the goal is reached, the curve will smoothly change the target to BackGoal,
/// which is the original value of the target by default.
/// Then call
template <typename T>
struct Simple_Curve
{
    Simple_Curve(T*targ,T gol,float alph=0.8,bool BGEn=false)
    {
        backGoal = *target;
        goal = gol;
        alpha = alph;
        BGEnable=BGEn;
    }

    bool BGEnable;

    float alpha;

    T goal;
    T backGoal;

    T*target;

    virtual void calculate()
    {
        if(BGEnable && *target == goal)
        {
            T temp = backGoal;
            backGoal = goal;
            goal = temp;
        }
        *target = *target * alpha + goal * (1-alpha);
    }
};


template <typename T>
class MultiCurve
{
    std::list<T*>targets;

    T goal;

    float alpha;

    bool BGEnable;

    void calculate()
    {
        for(auto a : targets)
        {
            if(BGEnable && a == goal)
            {
                *a = *a * alpha + goal*(1-alpha);
            }
        }
    }
};


#endif // CURVES_H
