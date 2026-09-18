# Instagram Unfollowers Tracker
A C++ program that parses your exported Instagram data to generate a list of users you follow who do not follow you back.

# Instructions:
1. **Export your data:** On Instagram, go to Settings > Account Center > Your information and permissions > Download your information. Request the "Followers and following" data strictly in **JSON format**.
2. **Extract the files:** Unzip the downloaded folder and locate `followers_1.json` and `following.json`.
3. **Setup:** Place both JSON files in the same directory as the executable program.
4. **Run:** Execute the compiled C++ program.
5. **Result:** The program will output an `unfollowers_list.txt` file containing the accounts that don't follow you back.
