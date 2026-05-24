import subprocess
from itertools import product

EXECUTABLE = "main.exe"

N_REPEAT = 5

PROTOCOLS = [
    "Random",
    "Reverse",
    "AlmostSorted",
]

ALGORITHMS = [
    "merge",
    "string_merge",
    "quick",
    "string_quick",
    "msb_radix_quick",
    "msb_radix",
]

OUTPUT_FILE = "results.csv"


def main():
    for protocol, algorithm in product(PROTOCOLS, ALGORITHMS):
        print(f"Running: protocol={protocol}, algorithm={algorithm}")

        cmd = [
            EXECUTABLE,
            str(N_REPEAT),
            protocol,
            algorithm,
            OUTPUT_FILE,
        ]

        try:
            subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL)
        except subprocess.CalledProcessError as e:
            print(f"FAILED: {protocol} {algorithm}")
            print(e)


if __name__ == "__main__":
    main()