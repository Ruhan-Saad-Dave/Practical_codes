"""
In 2nd year, CS class group A students play cricket, group B students play Badminton and
group C students play football. Write a python program using function to compute the
following:
1. list of students who play both cricket and badminton
2. list of students who play either criket or badminton but no both.
3. list of students who play neither circket nor badminton.
4. list of students who play cricket and football but not badminton
"""

def union(set_a, set_b):
    result = set_a.copy()
    for element in set_b:
        if element not in result:
            result.append(element)
    return result

def intersection(set_a, set_b):
    result = []
    for element in set_a:
        if element in set_b:
            result.append(element)
    return result

def difference(set_a, set_b):
    result = []
    for element in set_a:
        if element not in set_b:
            result.append(element)
    return result

def remove_duplicate(set_a):
    result = []
    for element in set_a:
        if element not in result:
            result.append(element)
    return result

cricket = []
badminton = []
football = []

print("Press q or quit to exit.")
while True:
    student = input("Enter student who play cricket:")
    if student.lower() in ['q', 'quit']:
        break
    else:
        cricket.append(student)

print("\n\nPress q or quit to exit.")
while True:
    student = input("Enter student who play badminton:")
    if student.lower() in ['q', 'quit']:
        break
    else:
        badminton.append(student)
print("\n\nPress q or quit to exit.")

while True:
    student = input("Enter student who play football:")
    if student.lower() in ['q', 'quit']:
        break
    else:
        football.append(student)

cricket = remove_duplicate(cricket)
badminton = remove_duplicate(badminton)
football = remove_duplicate(football)
print("\nCricket:", cricket)
print("Badminton:", badminton)
print("Football:", football)

#both cricket and badminton, cricket U badminton
print("\nStudents playing both Cricket and badminton:\n", intersection(cricket, badminton))

#either cricket or badminton but not both, (cricket U badminton) - (cricket N badminton)
print("\nStudents playing either Cricket or Badminton but not both:\n", difference(union(cricket, badminton), intersection(cricket, badminton)))

#neither cricket nor badminton, footbal - (cricket U badminton)
print("\nStundents playing neither Cricket nor Badminton:\n", difference(football, union(cricket, badminton)))

#cricket and football but not badminton, (cricket N football) - badminton
print("\nStudents playing Cricket and Football but not badminton:\n", difference(union(cricket, football), badminton))
