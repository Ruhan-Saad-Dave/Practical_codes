# rpc_client.py
# This program acts as the client, calling the remote functions hosted by the server.

import xmlrpc.client

# Create a proxy object that connects to the RPC server.
# The URL should match the server's address and port.
# This proxy object makes the remote functions feel like they are local.
try:
    proxy = xmlrpc.client.ServerProxy("http://localhost:8000/RPC2")
    print("Successfully connected to RPC server.")

    # --- Calling Remote Procedures ---
    # Now we can call the functions defined on the server as if they were
    # methods of the 'proxy' object.

    # 1. Call the 'greet' function
    name = "Student"
    greeting_message = proxy.greet(name)
    print(f"\nCalling greet('{name}')...")
    print(f"Server Response: {greeting_message}")

    # 2. Call the 'add' function
    num1, num2 = 10, 5
    sum_result = proxy.add(num1, num2)
    print(f"\nCalling add({num1}, {num2})...")
    print(f"Server Response: {sum_result}")

    # 3. Call the 'subtract' function
    num3, num4 = 100, 45
    subtract_result = proxy.subtract(num3, num4)
    print(f"\nCalling subtract({num3}, {num4})...")
    print(f"Server Response: {subtract_result}")

    # 4. Call the 'factorial' function
    num5 = 6
    factorial_result = proxy.factorial(num5)
    print(f"\nCalling factorial({num5})...")
    print(f"Server Response: {factorial_result}")

    # 5. Demonstrate error handling from the server
    num6 = -5
    print(f"\nCalling factorial({num6}) to test server-side error handling...")
    try:
        proxy.factorial(num6)
    except xmlrpc.client.Fault as err:
        print("Caught an error from the server as expected:")
        print(f"  Fault code: {err.faultCode}")
        print(f"  Fault string: {err.faultString}")


except ConnectionRefusedError:
    print("\nError: Connection refused.")
    print("Please make sure the 'rpc_server.py' is running before you run this client.")
except Exception as e:
    print(f"An unexpected error occurred: {e}")
