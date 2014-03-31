//Generated by the Argon Build System

/**
 * @brief
 * This file implements an abstract interface for controlling the music player
 * on certain devices. It serves as a hint and can be arbitrarily ignored by the
 * Hardware Abstraction Layer(HAL).
 *
 * @author Skyler Saleh
 * @file UserMusic.h
 **/
#ifndef UserMusic_h
#define UserMusic_h
namespace Argon {
//! @file UserMusic.h
//! @enum UserMusicState
//! An enum to represent the current state of the music player.
enum UserMusicState {
    kMusicStateNone,          /*!< Their is no current state*/
    kMusicPlaying,            //!< The user is currently listening to their own music.
    kMusicPaused,             //!< The user is listening to their music, but it is paused.
    kMusicCreatePlaylist,     //!< The user is creating a playlist of their music that they would like to play.
    kMusicNext,               //!< Moving to the next song.
    kMusicPrevious,           //!< Moving to the previous song.
    kMusicStopped             //!< User has music, but no playlist is set.
};
/**
 * @brief
 * This class implements an abstract interface for controlling the music player
 * on certain devices. It serves as a hint and can be arbitrarily ignored by the
 * Hardware Abstraction Layer(HAL).
 **/
class UserMusic {
private:
    static UserMusicState state;
    static bool supported_;
    UserMusicState triggered_state;
public:
    UserMusic() : triggered_state(kMusicStateNone){}
    ~UserMusic(){}

    //! This is used by the HAL to tell the application whether User Music is supported.
    static void set_supported_(bool b){
        supported_ = b;
    }
    //! This is used by the hal to explicitly set the current state.
    static void set_state_(UserMusicState s){
        state = s;
    }

    bool set_state(UserMusicState s);         //!<Sets the users music state. Returns true when the operation is completed
    static UserMusicState get_state(){
        return state;
    }                                                         //!<Gets the current Music State
    bool create_playlist(){
        return set_state(kMusicCreatePlaylist);
    }                                                                        //!<Opens a UI for music playlist selection
    bool play(){
        return set_state(kMusicPlaying);
    }                                                      //!<Plays users music
    bool stop(){
        return set_state(kMusicStopped);
    }                                                      //!<stops users music
    bool pause(){
        return set_state(kMusicPaused);
    }                                                      //!<pauses users music
    bool next(){
        return set_state(kMusicNext);
    }                                                   //!<advances to the next song
    bool prev(){
        return set_state(kMusicPrevious);
    }                                                       //!<returns to the previous song

    bool is_supported(){
        return supported_;
    }                                                //!<returns true if the HAL supports user music
    bool is_playing(){
        return state==kMusicPlaying;
    }                                                        //!<returns true if users music is playing
    bool is_stopped(){
        return state==kMusicStopped;
    }                                                        //!<returns true if the users music is stopped

};
};

#endif // ifndef Neon_Rush_ARUserMusic_h
