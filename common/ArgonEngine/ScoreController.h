/**
 * @brief This file contains classes for a basic high-score/achievement system.
 * @file ScoreController.h
 * @author Skyler Saleh, Ian Wiggins
 **/
#ifndef ScoreController_h
#define ScoreController_h
#include "TypeInfo.h"
#include "Utility.h"
#include <map>
#include <vector>
#include "ContainerReflection.h"
namespace  Argon {
//! @brief This class represents a recorded score.
struct ScoreControllerScore :public ReflectionBase{
    double score;     //!< The players score as a double.
    uint64_t daily_ranking;     //!< The players current daily ranking in the online high score system.
    uint64_t weekly_ranking;    //!< The players current weekly ranking in the online high score system.
    uint64_t all_time_ranking;      //!< The players all-time ranking in the online high score system.
    Argon::StringIntern leader_board;    //!< The leaderboard that the score belongs to.
    bool operator<(const ScoreControllerScore &s) const {
        if(score!=s.score) return score>s.score;
        return leader_board>s.leader_board;
    }

    bool operator==(const ScoreControllerScore &s) const {
        return score==s.score&& leader_board==s.leader_board;
    }
    bool operator!=(const ScoreControllerScore &s) const {
        return score!=s.score|| leader_board!=s.leader_board;
    }
    MAKE_VISIT_HEAD(Score);
};
//! @brief This class represents an achievement's current progress.
struct ScoreControllerAchievement :public ReflectionBase{
    double percent;     //!< How much percent is completed of the achievement.
    Argon::StringIntern achievement;     //!< The name of the achievement.
    MAKE_VISIT_HEAD(Achievement)
};
/**
 * @brief An implementation of a basic high-score/achievement system.
 * @details
 * This class provides many common high score related functionality. And also forms an interface
 * with global high-score systems handled by the HAL.
 *
 * It provides functionality for:
 * - Loading, Saving, Pushing, and Pulling scores from the disk and online high score systems.
 * - Signalling to the HAL the display of platform specific leader boards/achievement systems.
 * - Multiple High Score boards and achievements.
 * - Preloading default scores/ achievements.
 * - Automatic Score sorting.
 **/

class ScoreController:public ReflectionBase {
    friend struct TypeInfo<ScoreController>;
    static bool displayAchievments;
    static bool displayLeaderboard;
    static bool updateGlobalRankings;

public:

private:
    static std::map<Argon::StringIntern, ScoreControllerScore>  global_rankings;
    static std::map<Argon::StringIntern, double> achievements;
    static std::vector<ScoreControllerAchievement>       unsubmitted_achievements;
    static std::vector<ScoreControllerScore> scores;
    static std::vector<ScoreControllerScore> unsubmitted_scores;

public:
    static uint64_t total_global_scores;     //!< The total number of global scores.

    //! Tells the HAL to update the global rankings.
    static void update_global_rankings(){
        updateGlobalRankings = true;
    }

    //! Queried by the HAL. Returns true if the HAL should update global rankings.
    static bool should_update_global_rankings(){
        if(updateGlobalRankings) {
            updateGlobalRankings = false;
            return true;
        }
        return false;
    }
    //! Gets the users highest global score in leaderboard 's'.
    static ScoreControllerScore &get_global_ranking(const Argon::StringIntern& s){
        return global_rankings[s];
    }
    static void display_achievments(){
        displayAchievments = true;
    }                                                                  //!< Displays an achievment model view.
    static void display_leaderboard(){
        displayLeaderboard = true;
    }                                                                  //!< Displays a leaderboard model view.

    //! Submits a score to the local and global leaderboard 'board'.
    static void submit_score(double points, const Argon::StringIntern& board);

    //! @brief Loads a score into the local leaderboard 'board'. Leaving the global one unchanged.
    //! Use this to specify default scores.
    static void load_score(double points, const Argon::StringIntern& board);

    static void update_achievement(const Argon::StringIntern &str,double percent);    //!< Update the users progress on achievement "str" to percent 'percent'

    //! Returns the value of the top local score in leaderboard s.
    static double get_top_local_score(const Argon::StringIntern &s);
    static const std::vector<ScoreControllerScore>& get_scores(){
        return scores;
    }                                                                  //!< Returns a sorted vector of local high scores.
    static double get_achievement(const Argon::StringIntern &str){
        return achievements[str];
    }                                                                                            //!<returns the users current progress on achievement "str"
    void save_scores();    //!< Saves the highscore data to disk.
    void reload();         //!< Reloads the highscore data from disk and global boards.

    /////////////////////////////////////////////////////////////////////
    //    The following functions are used by the Abstraction Layer.   //
    /////////////////////////////////////////////////////////////////////

    //! Returns true if the HAL should display the leaderboards.
    static bool should_display_leaderboard(){
        bool b = displayLeaderboard; displayLeaderboard=false; return b;
    }
    //! Returns true if the HAL should display an achievements modal.
    static bool should_display_achievments(){
        bool b = displayAchievments; displayAchievments=false; return b;
    }
    //! Returns a vector of all achievements that have not been submitted.
    static const std::vector<ScoreControllerAchievement>& get_unsubmitted_achievements(){
        return unsubmitted_achievements;
    }
    //! Returns a vector of all scores that have not been submitted.
    static const std::vector<ScoreControllerScore>& get_unsubmitted_scores(){
        return unsubmitted_scores;
    }

    //! Clears the vector of all unsubmitted scores. Call this once the HAL has uploaded all unsubmitted scores.
    static void clear_unsubmitted_scores();

    //! Clears the vector of all unsubmitted achievements. Call this once the HAL has uploaded all unsubmitted achievements.
    static void clear_unsubmitted_achievements();
    MAKE_VISIT_HEAD(ScoreController);

};


}
#endif /* defined(__Neon_Rush_2__ARScoreController__) */
