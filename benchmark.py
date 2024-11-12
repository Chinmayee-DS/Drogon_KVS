import threading
import queue
from typing_extensions import KeysView
import requests
import time
import random
import hashlib

class ConsistentHash:
    def __init__(self, nodes=None):
        self.nodes = nodes or []
        self.ring = {}
        self._build_ring()
        self.sorted_keys = sorted(self.ring.keys())

    def _hash(self, key):
        return hashlib.md5(str(key).encode()).hexdigest()

    def _build_ring(self):
        self.ring = {self._hash(node): node for node in self.nodes}

    def get_node(self, key):
        if not self.ring:
            return None
        hash_key = self._hash(key)
        # for node_key in sorted(self.ring.keys()):
        for node_key in self.sorted_keys:
            if node_key >= hash_key:
                return self.ring[node_key]
        return self.ring[self.sorted_keys[0]]

# The base URL of the Flask server
# BASE_URL = 'http://127.0.0.1:8080'

SERVER_ONE = 'http://127.0.0.1:8080'
SERVER_TWO = 'http://127.0.0.2:8081'
SERVER_THREE = 'http://127.0.0.3:8081'
SERVER_FOUR = 'http://127.0.0.4:8081'

ch = ConsistentHash([SERVER_ONE, SERVER_TWO, SERVER_THREE, SERVER_FOUR])
# ch = ConsistentHash([SERVER_ONE, SERVER_TWO, SERVER_THREE])
# ch = ConsistentHash([SERVER_ONE, SERVER_TWO])
# ch = ConsistentHash([SERVER_ONE])

# Configure the number of threads and operations
NUM_THREADS = 3
OPS_PER_THREAD = 2000
PRINT_INTERVAL = 10  # Interval for printing intermediate results

# Queues for managing operations and latencies
operations_queue = queue.Queue()
latencies_queue = queue.Queue()

# Synchronize the starting of threads
start_event = threading.Event()

# Client operation function
# def kv_store_operation(op_type, key, value=None):
def kv_store_operation(op_type, key, URL, value=None):
    try:
        if op_type == 'set':
            # URL = ch.get_node(key)
            response = requests.post(f"{URL}/{key}", json={'value': value})
        elif op_type == 'get':
            # URL = ch.get_node(key)
            response = requests.get(f"{URL}/{key}")
        else:
            raise ValueError("Invalid operation type")
        # print(f"the response sent is {response}")
        response.raise_for_status()  # This will raise an error for non-2xx responses
        return True
    except Exception as e:
        print(f"Error during {op_type} operation for key '{key}' and value '{value}': {e}")
        # print(f"the response sent is {response}")
        return False

# kv_store_operation("set", "key_1", "val_1")
# kv_store_operation("get", "key_1")

# Worker thread function
def worker_thread():
    while not start_event.is_set():
        # Wait until all threads are ready to start
        pass

    while not operations_queue.empty():
        # op, key, value = operations_queue.get()
        op, key, url, value = operations_queue.get()
        start_time = time.time()
        # if kv_store_operation(op, key, value):
        if kv_store_operation(op, key, url, value):
            latency = time.time() - start_time
            latencies_queue.put(latency)

# Monitoring thread function
def monitor_performance():
    last_print = time.time()
    while True:
        time.sleep(PRINT_INTERVAL)
        current_time = time.time()
        elapsed_time = current_time - last_print
        latencies = []
        while not latencies_queue.empty():
            latencies.append(latencies_queue.get())

        if latencies:
            avg_latency = sum(latencies) / len(latencies)
            throughput = len(latencies) / elapsed_time
            print(f"[Last {PRINT_INTERVAL} seconds] Throughput: {throughput:.2f} ops/sec, "
                  f"Avg Latency: {avg_latency:.5f} sec/ops")
        last_print = time.time()

# Populate the operation queue with mixed 'set' and 'get' requests
for i in range(NUM_THREADS * OPS_PER_THREAD):
    # if i % 2 == 0:
    #     op_type = 'set'
    #     key = f"key_{i}"
    #     value = f"value_{i}"
    #     operations_queue.put((op_type, key, value))
    # else:
    #     op_type = 'get'
    #     key = f"key_{i-1}"
    #     value = None
    # operations_queue.put((op_type, key, value))
    # op_type = 'set' if i % 2 else 'get'
    # op_type = 'set'
    # key = f"key_{i}"
    # value = f"value_{i}" if op_type == 'set' else None
    # URL = ch.get_node(key)
    # # if(URL != SERVER_ONE):
    # #     print(URL)
    # # operations_queue.put((op_type, key, value))
    # operations_queue.put((op_type, key, URL, value))
    op_type = 'get'
    key = f"key_{i}"
    value = f"value_{i}" if op_type == 'set' else None
    URL = ch.get_node(key)
    # if(URL != SERVER_ONE):
    #     print(URL)
    # operations_queue.put((op_type, key, value))
    operations_queue.put((op_type, key, URL, value))

# Create and start worker threads
threads = [threading.Thread(target=worker_thread) for _ in range(NUM_THREADS)]

# Start the monitoring thread
monitoring_thread = threading.Thread(target=monitor_performance, daemon=True)
monitoring_thread.start()

# Starting benchmark
start_time = time.time()
start_event.set()  # Signal threads to start

for thread in threads:
    thread.start()

for thread in threads:
    thread.join()

# Calculate final results
total_time = time.time() - start_time
# total_ops = NUM_THREADS * OPS_PER_THREAD * 2 # times two for 'set' and 'get'
total_ops = NUM_THREADS * OPS_PER_THREAD # times two for 'set' and 'get'
total_latencies = list(latencies_queue.queue)
average_latency = sum(total_latencies) / len(total_latencies) if total_latencies else float('nan')
throughput = total_ops / total_time

print("\nFinal Results:")
print(f"Total operations: {total_ops}")
print(f"Total time: {total_time:.2f} seconds")
print(f"Throughput: {throughput:.2f} operations per second")
print(f"Average Latency: {average_latency:.5f} seconds per operation")
