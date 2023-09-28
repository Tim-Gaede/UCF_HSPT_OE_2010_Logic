// Arup Guha
// 5/7/2010
// Solution to UCF Summer High School Programming Contest Problem: Logic

import java.util.*;
import java.io.*;

// A class to store a clue - I want access to everything, so the
// instance variables are public.
class clue {
	
	public String clue1;
	public String relation;
	public String clue2;
	
	public clue(String c1, String r, String c2) {
		clue1 = c1;
		relation = r;
		clue2 = c2;
	}
	
}
public class logic {
	
	// The four relationships in the problem.
	final public static String EQUALS  = "=";
	final public static String YOUNGER  = "<";
	final public static String OLDER  = ">";
	final public static String DIFFERENT  = "!";
	
	// Stores all the data for one puzzle.
	private String[] categories;
	private String[][] listing;
	private int[][] permutations;
	private clue[] theClues;
	private String[] diffPeople;
	
	// Creates a new logic object, passed a Scanner object which reads from
	// the beginning of the description of one puzzle.
	public logic(Scanner fin)  throws Exception {
		
		// Read in the number of categories & options.
		int numCategories = fin.nextInt();
		int numOptions = fin.nextInt();
		
		// Allocate all the space.
		categories = new String[numCategories];
		listing = new String[numCategories][numOptions];
		permutations = new int[numCategories][numOptions];
		
		// Now read everything in.
		for (int i=0; i<numCategories; i++)
			categories[i] = fin.next();

		for (int i=0; i<numCategories; i++)
			for (int j=0; j<numOptions; j++)
				listing[i][j] = fin.next();
				
		// Set up initial permutations.
		for (int i=0; i<numCategories; i++)
			for (int j=0; j<numOptions; j++)
				permutations[i][j] = j;
				
		// Read in all the clues.
		int numClues = fin.nextInt();
		theClues = new clue[numClues];
		
		for (int i=0; i<numClues; i++) {
			String clue1 = fin.next();
			String relation = fin.next();
			String clue2 = fin.next();
			theClues[i] = new clue(clue1, relation, clue2);
		}
		
		diffPeople = new String[numOptions];
		for (int i=0; i<numOptions; i++)
			diffPeople[i] = fin.next();
	}
	
	// Returns a String representation of this object according to the problem
	// specification.
	public String toString() {
		String answer = "";
		
	        for (int j=0; j<categories.length; j++) 
       		{
			if (j > 0) answer = answer + " ";
			answer = answer + categories[j];
		}
		answer = answer + "\n";

		// Loop through each line.
		for (int i=0; i<listing[0].length; i++) {
			
			// Put the rest of the categories into this line.
			for (int j=0; j<listing.length; j++) {

            if (j != 0)
               answer = answer + " ";
				answer = answer + listing[j][permutations[j][i]];
         }

			answer = answer + "\n";
		}
		
		return answer;
	}
	
	public boolean isConsistent() {
		
		// Go through each clue.
		for (int i=0; i<theClues.length; i++) {
			
			int index1 = getIndex(theClues[i].clue1);
			int index2 = getIndex(theClues[i].clue2);
			String relation = theClues[i].relation;
			
			// Check for the four possible inconsistencies that
			// might occur due to this clue and return false if
			// any of them have occurred.
			if (relation.equals(EQUALS) && index1 != index2)
				return false;
			if (relation.equals(YOUNGER) && index1 >= index2)
				return false;
			if (relation.equals(OLDER) && index1 <= index2)
				return false;
			if (relation.equals(DIFFERENT) && index1 == index2)
				return false;
		}
		
		// Now check the different people.
		int[] indexes = new int[diffPeople.length];
		for (int i=0; i<diffPeople.length; i++)
			indexes[i] = getIndex(diffPeople[i]);
			
		// If two of these people are the same, it doesn't work!
		for (int i=0; i<diffPeople.length; i++)
			for (int j=i+1; j<diffPeople.length; j++)
				if (indexes[i] == indexes[j])
					return false;
		
		// If we get here, we never found an inconsistency and
		// this solution works!
		return true;
	}
	
	// Returns the option number where clue is stored.
	public int getIndex(String clue) {
		
		for (int i=0; i<listing.length; i++)
			for (int j=0; j<listing[i].length; j++)
				
				// What we want is the index into the perm array
				// where j (the number of this string) is stored.
				if (listing[i][j].equals(clue))
					return find(permutations[i],j);
				
		// Should never get here, but Java needs this to compile.	
		return 0;
			
	}
	
	// Returns the index in perm that stores val.
	public static int find(int[] perm, int val) {
		for (int i=0; i<perm.length; i++)
			if (perm[i] == val)
				return i;
		
		// Should never get here.
		return 0;
	}
	
	// Solves the puzzle.
	public void solve() {
		
		// If we have a category to permute.
		if (categories.length > 1) {
		
			for (int i = 0; i<fact(listing[1].length); i++) {
			
				// If we have a second category to permute
				if (categories.length > 2) {
					
					for (int j=0; j<fact(listing[2].length); j++) {
						
						// If we have a third category to permute
						if (categories.length > 3) {
							
							for (int k=0; k<fact(listing[3].length); k++) {
								
								if (isConsistent())
									return;
								
								nextPerm(permutations[3]);
							}
						}
												
						if (isConsistent())
							return;
						nextPerm(permutations[2]);
					}
				}
				
				if (isConsistent())
					return;
				nextPerm(permutations[1]);
			}
		}
	}
	
	// Changes perm so it stores the next lexicographical permutation
	// of the integers stored in there, assuming they store 0, 1, 2, ...,
	// perm.length-1.
	public static void nextPerm(int[] perm) {
		
	    // Find the spot that needs to change.
        int i = perm.length-1;
    	while (i>0 && perm[i] < perm[i-1]) i--;
    	i--; // Advance to the location that needs to be swapped.
		
    	// So last perm needs to go back to the first, just reverse the whole list!
    	if (i == -1) {
    		
    		for (int j=0; j<perm.length/2; j++) {
    			int temp = perm[j];
    			perm[j] = perm[perm.length-1-j];
    			perm[perm.length-1-j] = temp;
    		}
    		return;
    	}
		
    	// Find the spot with which to swap index i.
    	int j=perm.length-1;
    	while (j>i && perm[j]<perm[i]) j--;
		
    	// Swap it.
    	int temp = perm[i];
    	perm[i] = perm[j];
    	perm[j] = temp;
		
   		// reverse from index i+1 to length-1.
  	    int k,m;
   		for (k=i+1,m=perm.length-1; k<m; k++,m--) {
            temp = perm[k];
       		perm[k] = perm[m];
       		perm[m] = temp;
    	}
	}
	
	// Returns n!
	public static int fact(int n) {
		int ans = 1;
		for (int i=2; i<=n; i++)
			ans = ans*i;
		return ans;
	}
	
	public static void main(String[] args) throws Exception {
		
		Scanner fin = new Scanner(System.in);
		int numCases = fin.nextInt();
		
		// Go through each case!
		for (int i=1; i<=numCases; i++) {
			logic brainteaser = new logic(fin);
			brainteaser.solve();
			System.out.println("Logic Puzzle #"+i+":\n"+brainteaser);
		}
	}
}
