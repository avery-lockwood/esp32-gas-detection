import asyncio
import websockets

async def handle_websocket(websocket, path):
    while True:
        try:
            data = await websocket.recv()
            print(f"Received data from client: {data}")

            # You can process the data as needed before sending it back to the client
            # In this example, we just send the received data back to the client
            await websocket.send(data)
        except websockets.ConnectionClosed:
            print("WebSocket connection closed")
            break

start_server = websockets.serve(handle_websocket, "localhost", 8080)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
