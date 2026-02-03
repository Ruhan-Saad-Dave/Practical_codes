import pandas as pd 
path = "datasets/13adult.csv"
df = pd.read_csv(path)
print("Raw dataset")
print(df)
print("\n" * 5)

print("Is NULL?")
print(df.isnull())
print("\n" * 5)

print("describe")
print(df.describe())
print("\n" * 5)

print("Data types")
print(df.dtypes)
print("\n" * 5)

print("Shape")
print(df.shape)
print("\n" * 5)

print("Size")
print(df.size)
print("\n" * 5)

print("Length")
print(len(df))
print("\n" * 5)

print("Head")
print(df.head(5))
print("\n" * 5)

print("Tail")
print(df.tail(5))
print("\n" * 5)

