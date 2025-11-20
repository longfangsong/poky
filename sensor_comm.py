#!/usr/bin/env python3
import sys
import subprocess

THRESHOLD = 1_000_000_000

def read_sensor():
    dev = "/dev/sensor"
    try:
        with open(dev, "rb") as f:
            data = f.read(4)
    except OSError as e:
        print(f"open /dev/sensor: {e}", file=sys.stderr)
        return None

    if len(data) == 4:
        # Interpret using native byte order to mirror the C++ behavior
        value = int.from_bytes(data, byteorder=sys.byteorder, signed=False)
        return value

def main():
    value = read_sensor()
    if value is None:
        return -1
    
    print(value)

    if value < THRESHOLD:
        try:
            result = subprocess.run(["communicate", str(value)])
            return result.returncode
        except Exception as e:
            print(f"fork/exec error: {e}", file=sys.stderr)
            return -1

    return 0


if __name__ == "__main__":
    sys.exit(main())
