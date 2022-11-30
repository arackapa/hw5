

#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    int day,
    int shift
); 
void scheduleInitializer(const AvailabilityMatrix& avail, DailySchedule& sched, const size_t dailyNeed); 
bool isValid(const AvailabilityMatrix& avail, DailySchedule& sched, const size_t maxShifts); 

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    scheduleInitializer(avail, sched, dailyNeed); 
    bool joseph = scheduleHelper(avail, dailyNeed, maxShifts, sched, 0, 0);
    return joseph; 
}

// helper function to determine if a placement is valid 
bool isValid(const AvailabilityMatrix& avail, DailySchedule& sched, const size_t maxShifts)
{
  // outer for loop to go through each worker
  for (int i = 0; i < (int)avail[0].size(); i++)
  {
    int count = 0; 
    // inner for loop to check each day 
    for (int j = 0; j < (int)avail.size(); j++)
    {
      // seeing for each worker, how many days they are scheduled 
      // not a valid placement if the worker is scheduled twice in one day 
      if (std::count(sched[j].begin(), sched[j].end(), i) > 1) 
      {
        return false; 
      }

      // seeing that if we find an instance of the worker that week, we increment the counter 
      if (std::find(sched[j].begin(), sched[j].end(), i) != sched[j].end())
      {
        count ++; 
      }
    }
    if (count > (int)maxShifts) 
    {
      // not a valid placement if the person has too many shifts in the week 
      return false; 
    }
  }
  // valid placement if we get here 
  return true; 
}

// initializing the schedule to the proper size so we can access easily later 
// filling with -1 which converted to an unsigned int is some large number 
void scheduleInitializer(const AvailabilityMatrix& avail, DailySchedule& sched, const size_t dailyNeed) 
{
  for (int i = 0; i < (int)avail.size(); i++)
  {
    sched.push_back(std::vector<Worker_T>());
    for (int j = 0; j < (int)dailyNeed; j++)
    {
      sched[i].push_back(-1); 
    }
  }
}

// helper function to keep track of the days and shifts 
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    int day,
    int shift
)
{
  // we want to recurse through sched 
  // if the number of days == to the day we are looking at return true, we have made it 
  if ((int)sched.size() == day) 
  { 
    return true; 
  }

  // if we make it to the end of a column, we must adjust day and shift 
  // recurse 
  if ((int)dailyNeed == shift)
  {
    day += 1; 
    shift = 0; 
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, day, shift); 
  }

  // we have the proper day, we just need to insert a worker at that slot!
  for (int i = 0; i < (int)avail[0].size(); i++)
  {
    if (avail[day][i] == 1)
    {
      sched[day][shift] = i;
      if (isValid(avail, sched, maxShifts))
      {
        if(scheduleHelper(avail, dailyNeed, maxShifts, sched, day, shift + 1)) 
        {
          return true;
        }
      }
      sched[day][shift] = -1; 
    }
  }
  return false; 
}