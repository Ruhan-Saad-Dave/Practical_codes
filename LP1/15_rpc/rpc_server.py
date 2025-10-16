# rpc_server.py
# This program acts as the server, hosting functions that can be called by a remote client.

from xmlrpc.server import SimpleXMLRPCServer
from xmlrpc.server import SimpleXMLRPCRequestHandler

# Restrict to a particular path.
class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

# Create server
# The server is bound to 'localhost' and will listen on port 8000.
with SimpleXMLRPCServer(('localhost', 8000),
                        requestHandler=RequestHandler) as server:
    server.register_introspection_functions()

    print("RPC Server is running on port 8000...")
    print("You can now run the rpc_client.py in a separate terminal.")

    # --- Remote Procedures ---
    # These are the functions that the client can call.
    # They are defined here on the server.

    # A simple function to return a greeting string.
    def greet(name):
        """Returns a personalized greeting message."""
        print(f"Server received a call to greet('{name}')")
        return f"Hello, {name}! This message is from the remote server."

    # Register the 'greet' function so it's accessible via RPC.
    server.register_function(greet, 'greet')


    # A function to add two numbers.
    def add(x, y):
        """Adds two integer numbers."""
        print(f"Server received a call to add({x}, {y})")
        return x + y

    # Register the 'add' function.
    server.register_function(add, 'add')


    # A function to subtract two numbers.
    def subtract(x, y):
        """Subtracts the second number from the first."""
        print(f"Server received a call to subtract({x}, {y})")
        return x - y
    
    # Register the 'subtract' function.
    server.register_function(subtract, 'subtract')


    # A function to calculate the factorial of a number.
    def factorial(n):
        """Calculates the factorial of a non-negative integer."""
        print(f"Server received a call to factorial({n})")
        if n < 0:
            # RPC can also handle errors/faults.
            raise ValueError("Factorial is not defined for negative numbers.")
        elif n == 0:
            return 1
        else:
            res = 1
            for i in range(1, n + 1):
                res *= i
            return res

    # Register the 'factorial' function.
    server.register_function(factorial, 'factorial')


    # Run the server's main loop to listen for requests.
    # The server will run indefinitely until you stop it (e.g., with Ctrl+C).
    server.serve_forever()
