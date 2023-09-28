
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Item
{
   char   value[64];
};


struct Person
{
   // One item for each category
   int   trait[8];
};


struct Clue
{
   int   category1;
   int   trait1;

   char  op;

   int   category2;
   int   trait2;
};


// We're guaranteed that Age is always category 0
const int   AGE = 0;


int      numCategories, numOptions;
char     categoryEcho[256];
char     categoryNames[8][64];
Item     options[8][8];
int      ages[8];

Person   people[8];
bool     used[8][8];
bool     fixed[8][8];

int      numClues;
Clue     clues[128];

int      puzzle;


void outputSolution()
{
   int      i, j, numPass;
   Person   temp;
   bool     exch;
   int      age1, age2;

   // Number of sorting passes
   numPass = numOptions - 1;

   // Sort the people by age
   exch = true;
   while (exch)
   {
      // No exchanges yet
      exch = false;
      for (i = 0; i < numPass; i++)
      {
         age1 = ages[people[i].trait[AGE]];
         age2 = ages[people[i+1].trait[AGE]];
         if (age1 > age2)
         {
            temp = people[i];
            people[i] = people[i+1];
            people[i+1] = temp;
            exch = true;
         }
      }

      // One fewer pass on the next loop
      numPass--;
   }

   // Finally, output the solution
   printf("Logic Puzzle #%d:\n", puzzle);
   printf("%s", categoryEcho);
   for (i = 0; i < numOptions; i++)
   {
      for (j = 0; j < numCategories; j++)
      {
         printf("%s", options[j][people[i].trait[j]].value);

         if (j < numCategories-1)
            printf(" ");
      }

      printf("\n");
   }
}

bool search(int p, int c)
{
   int     i, j;
   bool    found;
   int     p1, p2;
   int     age1, age2;

   if (p > 0)
   {
      if (c > 0)
      {
         // Make sure this category isn't pre-defined for this person
         if (!fixed[p-1][c-1])
         {
            for (i = 0; i < numOptions; i++)
            {
               if (!used[c-1][i])
               {
                  // Try option i for category c on person p
                  people[p-1].trait[c-1] = i;
                  used[c-1][i] = true;

                  // Recursviely pick other options for the remaining
                  // categories
                  found = search(p, c-1);

                  // If we found a good solution down the last recursive
                  // branch, stop searching
                  if (found)
                     return true;

                  // We tried all possibilities with this option picked for
                  // category c on person p, now mark it unused so we can
                  // try it elsewhere
                  used[c-1][i] = false;
               }
            }
         }
         else
         {
            // Skip this category (we already know the right value for it)
            // and go straight to the next one
            found = search(p, c-1);

            // If we found a good solution on the last recursive branch,
            // we don't need to search anymore
            if (found)
               return true;
         }
      }
      else
      {
         // Recursively pick values for all categories on the next person
         found = search(p-1, numCategories);

         // If we found a good solution, we don't need search anymore
         if (found)
            return true;
      }
   }
   else
   {
      // Check this solution
      for (i = 0; i < numClues; i++)
      {
         // First, figure out which two people we're talking about
         for (j = 0; j < numOptions; j++)
         {
            if (people[j].trait[clues[i].category1] == clues[i].trait1)
               p1 = j;

            if (people[j].trait[clues[i].category2] == clues[i].trait2)
               p2 = j;
         }

         // Now make sure the constraint is satisfied
         switch (clues[i].op)
         {
            case '>':

               // Get the ages of the two people
               j = people[p1].trait[AGE];
               age1 = ages[j];
               j = people[p2].trait[AGE];
               age2 = ages[j];

               // Make sure age1 is greater than age2
               if (age1 <= age2)
                  return false;
               break;

            case '<':

               // Get the ages of the two people
               j = people[p1].trait[AGE];
               age1 = ages[j];
               j = people[p2].trait[AGE];
               age2 = ages[j];

               // Make sure age1 is less than age2
               if (age1 >= age2)
                  return false;
               break;

            case '=':

               // Make sure the two people are the same
               if (p1 != p2)
                  return false;
               break;

            case '!':

               // Make sure the two people are not the same
               if (p1 == p2)
                  return false;
               break;
         }
      }

      // If we get this far, we must have a valid solution
      outputSolution();

      // Return true, to indicate we don't need to search anymore
      return true;
   }

   // If we get this far, we haven't found a good solution yet, so keep
   // searching
   return false;
}


int getCategory(char * value)
{
   int i, j;

   // Search each category
   for (i = 0; i < numCategories; i++)
   {
      // Check each option within that category
      for (j = 0; j < numOptions; j++)
      {
         // See if this is the category and option we want
         if (strcmp(options[i][j].value, value) == 0)
            return i;
      }
   }

   // We didn't find it (this shouldn't happen)
   return -1;
}


int getOptionIndex(int category, char * value)
{
   int i;

   // Check each option within the specified category
   for (i = 0; i < numOptions; i++)
   {
      // See if this is the option we want
      if (strcmp(options[category][i].value, value) == 0)
         return i;
   }

   // We didn't find it (this shouldn't happen)
   return -1;
}


int main(void)
{
   FILE *   fp;
   char     line[128];
   int      numPuzzles;
   int      i, j;
   char *   token;
   int      category;

   // Open the input file
   fp = stdin;

   // Get the number of puzzles
   fgets(line, sizeof(line), fp);
   sscanf(line, "%d\n", &numPuzzles);

   // Read and solve each puzzle
   for (puzzle = 1; puzzle <= numPuzzles; puzzle++)
   {
      // Read the number of categories and options
      fgets(line, sizeof(line), fp);
      sscanf(line, "%d %d\n", &numCategories, &numOptions);

      // Read the category names
      fgets(line, sizeof(line), fp);
      strcpy(categoryEcho, line);
      token = strtok(line, " \n\r");
      for (i = 0; i < numCategories; i++)
      {
         // Copy the option into the array of options
         strcpy(categoryNames[i], token);

         // Parse the next category
         token = strtok(NULL, " \n\r");
      }

      // Read the options
      for (i = 0; i < numCategories; i++)
      {
         // Read the next line
         fgets(line, sizeof(line), fp);

         // Parse the first option from the line
         token = strtok(line, " \n\r");

         // Traverse the rest of the line
         for (j = 0; j < numOptions; j++)
         {
            // Copy the option into the array of options
            strcpy(options[i][j].value, token);

            // Parse the next option
            token = strtok(NULL, " \n\r");
         }
      }

      // Convert the ages to integers, so we don't have to do it repeatedly
      // later while we're searching for the solution
      for (i = 0; i < numOptions; i++)
      {
         ages[i] = atoi(options[AGE][i].value);
      }

      // Read the number of clues
      fgets(line, sizeof(line), fp);
      sscanf(line, "%d\n", &numClues);
      for (i = 0; i < numClues; i++)
      {
         // Read the next clue
         fgets(line, sizeof(line), fp);

         // Parse the clue, starting with the first trait
         token = strtok(line, " \n\r");

         // Get the category for this trait
         clues[i].category1 = getCategory(token);

         // Get the index of the option within that category and store it
         // (we store the index instead of the whole string because it's
         // quicker to compare integers than strings, and we're going to
         // be doing a lot of comparing)
         j = getOptionIndex(clues[i].category1, token);
         clues[i].trait1 = j;

         // Get the clue's operator
         token = strtok(NULL, " \n\r");
         clues[i].op = token[0];

         // Now, get the second trait, just like we did for the first one
         token = strtok(NULL, " \n\r");
         clues[i].category2 = getCategory(token);
         j = getOptionIndex(clues[i].category2, token);
         clues[i].trait2 = j;
      }

      // Zero out the fixed array (set all entries to false), which keeps
      // track of which trait is pre-defined on each person
      memset(fixed, 0, sizeof(fixed));

      // Also zero out the used array (set all entries to false), which
      // keeps track of which options we've selected already during our search
      memset(used, 0, sizeof(used));

      // Read the final clue (the unique traits)
      fgets(line, sizeof(line), fp);
      token = strtok(line, " \n\r");
      for (i = 0; i < numOptions; i++)
      {
         // Figure out which category this trait is in
         category = getCategory(token);

         // Figure out the index of the trait within that category's options
         j = getOptionIndex(category, token);

         // Pre-set the option for this category on this person
         people[i].trait[category] = j;

         // Mark this trait fixed, so we don't change it while we're
         // solving the puzzle
         fixed[i][category] = true;

         // Also mark the trait as a used option, so we don't try to
         // use it on other people
         used[category][j] = true;

         // Parse the next clue
         token = strtok(NULL, " \n\r");
      }

      // Now, search for a solution that satisfies all clues
      search(numOptions, numCategories);

      // Print the blank line following each puzzle
      printf("\n");
   }

   return 0;
}

