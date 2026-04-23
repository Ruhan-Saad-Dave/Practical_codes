print("Name = Ruhan Saad Dave")
print("Roll no = TECOA23101")

class StockMarketExpertSystem:
    def __init__(self):
        # The Knowledge Base represents facts and rules [cite: 1132]
        self.rules = []
        self.facts = {}

    def get_user_input(self):
        # User Interface module to interact with the non-expert user 
        print("--- Stock Market Indicator Input ---")
        self.facts['price_trend'] = input("Is the price trend Up, Down, or Stable? ").lower()
        self.facts['volume_trend'] = input("Is the volume trend Up or Down? ").lower()
        self.facts['news_sentiment'] = input("Is the news sentiment Positive or Negative? ").lower()
        self.facts['interest_rates'] = input("Are interest rates High or Low? ").lower()

    def inference_engine(self):
        # The Inference Engine fetches knowledge and interprets it to find a solution 
        # This implementation uses Forward Chaining for prediction [cite: 1145, 1148]
        
        advice = "Hold / Neutral"
        explanation = "Insufficient data to provide strong advice."

        # Rule 1: Bullish Scenario
        if self.facts['price_trend'] == 'up' and self.facts['volume_trend'] == 'up':
            advice = "Strong Buy"
            explanation = "Price and volume are both increasing, indicating strong upward momentum."
        
        # Rule 2: Bearish Scenario
        elif self.facts['price_trend'] == 'down' and self.facts['volume_trend'] == 'up':
            advice = "Strong Sell"
            explanation = "Price is falling while volume increases, indicating heavy selling pressure."

        # Rule 3: Sentiment-based Rule
        elif self.facts['news_sentiment'] == 'positive' and self.facts['interest_rates'] == 'low':
            advice = "Buy"
            explanation = "Positive sentiment and low interest rates favor market growth."

        # Rule 4: Interest Rate Risk
        elif self.facts['interest_rates'] == 'high':
            advice = "Sell / Caution"
            explanation = "High interest rates generally lead to a bearish movement in the stock market."

        return advice, explanation

    def display_result(self, advice, explanation):
        print("\n--- Expert System Advice ---")
        print(f"Recommendation: {advice}")
        
        # The Explanation Module provides details on the conclusion [cite: 1142]
        print(f"Explanation: {explanation}")

# Main execution
if __name__ == "__main__":
    # Simulate an expert system simulating human judgment in stock trading [cite: 1088, 1091]
    expert = StockMarketExpertSystem()
    expert.get_user_input()
    rec, reason = expert.inference_engine()
    expert.display_result(rec, reason)