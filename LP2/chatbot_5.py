print("Name = Ruhan Saad Dave")
print("Roll no = TECOA23101")

import nltk
import random
import string
import pickle
from nltk.stem import WordNetLemmatizer

# --- Pre-processing Setup ---
# Downloading necessary NLTK packages as per lab requirements [cite: 1008]
nltk.download('popular', quiet=True) 

# Initializing Lemmatizer to transform words into their lemma form 
lemmer = WordNetLemmatizer()

def LemTokens(tokens):
    return [lemmer.lemmatize(token) for token in tokens]

remove_punct_dict = dict((ord(punct), None) for punct in string.punctuation)

def LemNormalize(text):
    # Tokenizing the text into smaller chunks [cite: 1009]
    return LemTokens(nltk.word_tokenize(text.lower().translate(remove_punct_dict)))

# --- Knowledge Base (Retrieval-based) ---
# A retrieval-based chatbot works on pre-defined input patterns [cite: 968]
GREETING_INPUTS = ("hello", "hi", "greetings", "sup", "what's up", "hey")
GREETING_RESPONSES = ["hi", "hey", "*nods*", "hi there", "hello", "I am glad! You are talking to me"]

CUSTOMER_QUERY_BASE = {
    "hours": "Our business hours are 9 AM to 6 PM, Monday to Friday. [cite: 1023]",
    "address": "We are located at Sector No. 29, PCNTDA, Nigidi Pradhikaran, Akurdi, Pune. [cite: 8, 1023]",
    "reservation": "You can make a reservation by calling our help desk. [cite: 1020, 1025]",
    "status": "Your order is currently being processed and will be shipped soon. [cite: 1016]"
}

# --- Response Logic ---
def greeting(sentence):
    for word in sentence.split():
        if word.lower() in GREETING_INPUTS:
            return random.choice(GREETING_RESPONSES)

def response(user_response):
    # Elementary rule-based logic to pick appropriate responses [cite: 956, 1003]
    user_response = user_response.lower()
    robo_response = ''
    
    found = False
    for key in CUSTOMER_QUERY_BASE:
        if key in user_response:
            robo_response = CUSTOMER_QUERY_BASE[key]
            found = True
            break
            
    if not found:
        robo_response = "I am sorry, I don't understand that query. Could you please rephrase?"
    
    return robo_response

# --- Main Interaction Loop ---
# Chatbots provide 24x7 availability for customer satisfaction [cite: 1018, 1027]
flag = True
print("ROBO: My name is Robo. I will answer your queries about our services. If you want to exit, type Bye!")

while(flag == True):
    user_response = input("User: ")
    user_response = user_response.lower()
    if(user_response != 'bye'):
        if(user_response == 'thanks' or user_response == 'thank you'):
            flag = False
            print("ROBO: You are welcome..")
        else:
            if(greeting(user_response) != None):
                print("ROBO: " + greeting(user_response))
            else:
                print("ROBO: ", end="")
                print(response(user_response))
    else:
        flag = False
        print("ROBO: Bye! Take care..")

# Saving session data to a pickle file as described in manual 
with open('chatbot_model.pkl', 'wb') as f:
    pickle.dump(CUSTOMER_QUERY_BASE, f)