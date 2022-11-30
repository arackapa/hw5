// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
std::set<std::string> wordleHelper(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict, 
    std::set<std::string>& answers); 

std::set<std::string> wordleFinisher(
    const std::string& in,
    const std::set<std::string>& dict, 
    std::set<std::string>& answers);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
  set<string> answers; 
  if (dict.find(in) != dict.end())
  {
    answers.insert(in); 
  }
  else
  {
    answers = wordleHelper(in, floating, dict, answers); 
  }

    return answers;
}

// Define any helper functions here

std::set<std::string> wordleHelper(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict, 
    std::set<std::string>& answers)
{
  // go through the string to spot any dashes '-'
  for (int i = 0; i < (int)in.size(); i++)
  {
    if (in[i] == '-')
    {
      // once a dash is found, we go through the floating characters and place them accordingly to limit the number of combinations we try 
      string temp = in; 
      for(int j = 0; j < (int)floating.size(); j++)
      {
        string copy = floating;
        char limit = floating[j]; 
        temp[i] = limit; 
        copy.erase(j, 1); 
        wordleHelper(temp, copy, dict, answers); 
        // add the floating back so that through recursion all combinations fo the floating characters are tried
        copy.push_back(limit); 
      } 
      // if we have used up all of the floating digits, we should then test a-z in the remaining spaces
      if ((int)floating.size() == 0)
      {
        // merge the sets 
        std::set<std::string> specificAnswers = wordleFinisher(temp, dict, answers); 
        answers.insert(specificAnswers.begin(), specificAnswers.end()); 
      }
      // this is for the case where there is one dash left and it makes up the word 
      else if ((int)floating.size() == 1)
      {
        // check if its a legit word, then insert if so 
        if (dict.find(temp) != dict.end())
        {
          answers.insert(temp); 
        }
      }
    }
  }
  return answers; 
}

// once we have the floating characters inserted, we should generate all possible n length words that meet the restrictions 
std::set<std::string> wordleFinisher(
    const std::string& in,
    const std::set<std::string>& dict, 
    std::set<std::string>& answers)
{
  string temp = in; 
  // scan through rest of the word, determine where the dashes are 
  for (int i = 0; i < (int)in.size(); i++)
  {
    if (in[i] == '-')
    {
      char letter = 'a'; 
      // test every letter by recursion 
      for (int j = 0; j < 26; j++)
      {
        temp[i] = letter;
        letter ++; 
        wordleFinisher(temp, dict, answers); 
      }
    }
  }
  // check if its legit word , if so add to set 
  if (dict.find(in) != dict.end())
  {
    answers.insert(in); 
  }

  return answers; 
}
