import asyncio
import websockets
import ssl

async def handle_client(websocket, path):
    try:
        while True:
            # Receive data from the client
            data = await websocket.recv()
            print(f"Received data: {data}")

            # Send the received data back to the client
            await websocket.send(f"Server received: {data}")
    except websockets.exceptions.ConnectionClosedError:
        print("Client disconnected")

async def main():
    # Use SSL context to run the server on port 443
    ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ssl_context.load_cert_chain('your_certificate.crt', 'your_private_key.key')  # Replace with your actual certificate and private key paths

    server = await websockets.serve(
        handle_client,
        "0.0.0.0",  # Listen on all available network interfaces
        443,        # Port 443
        ssl=ssl_context
    )

    print("WebSocket server started. Listening on wss://localhost:443")

    await server.wait_closed()

if __name__ == "__main__":
    asyncio.run(main())