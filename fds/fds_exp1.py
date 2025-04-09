"""
Aim:
to write python code to store marks scored in subject "FDS" by 'N' number of students/

Objective:
1. average score
2. highest and lowest score
3. count of students who were absent
4. display marks with highest frequency
"""

student_score = []
print("Enter q, quit or exit to stop taking student scores.")
print("Enter -1 for absent students")
while True:
    score = input("Enter score of student:")
    if score.lower() in ['q', 'quit', 'exit']:
        break
    else:
        student_score.append(float(score))

print(student_score)

#average score
temp = student_score.copy()
count = 0
total_score = 0
for i in temp:
    if i == -1:
        temp.remove(-1)
    else:
        total_score += i
        count += 1
average_score = total_score / count
print("Average score = ", average_score)

# highest and lowest score
highest = -99999
lowest = 99999
for i in temp:
    if i > highest:
        highest = i
    if i < lowest:
        lowest = i
print("Highest score = ", highest)
print("Lowest score = ", lowest)

#count of students who are absent
absent_count = 0
for i in student_score:
    if i == -1:
        absent_count += 1
print("Absent students count =", absent_count)

#marks with highest frequency
score_frequency = {}
for i in temp:
    if i not in score_frequency:
        score_frequency[i] = 1
    else:
        score_frequency[i] += 1
highest_frequency = 0
highest_frequency_score = 0
for key, value in score_frequency.items():
    if value > highest_frequency:
        highest_frequency = value
        highest_frequency_score = key
print("Highest frequncy score is:", highest_frequency_score)
print("Frequncy is:", highest_frequency_score)
