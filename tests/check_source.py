#!/usr/bin/env python3
from pathlib import Path
import re
import sys

SOURCE = Path(__file__).resolve().parents[1] / "hw79.s"
text = SOURCE.read_text(encoding="utf-8")

ALLOWED_INSTRUCTIONS = {
    "movq", "addq", "subq", "imulq", "cmpq",
    "jmp", "je", "jne", "jl", "jle", "jg", "jge",
    "jb", "jbe", "ja", "jae",
    "pushq", "call", "leave", "ret",
}

FUNCTIONS = [
    "relation_code", "clamp_long", "weighted_sum8",
    "alternating_sum", "recursive_power", "combine_helpers",
]

ALLOWED_CALLS = {"recursive_power", "helper_square", "helper_affine"}
errors = []
found_labels = set()

for line_number, original_line in enumerate(text.splitlines(), start=1):
    line = original_line.split("#", 1)[0].strip().lower()
    if not line:
        continue
    if line.startswith("."):
        continue
    if line.endswith(":"):
        found_labels.add(line[:-1].strip())
        continue

    instruction = line.split(None, 1)[0]
    if instruction not in ALLOWED_INSTRUCTIONS:
        errors.append(
            f"line {line_number}: instruction '{instruction}' is not in the lecture whitelist"
        )

    if instruction == "call":
        parts = line.split()
        target = parts[1] if len(parts) >= 2 else "<missing>"
        if target not in ALLOWED_CALLS:
            errors.append(f"line {line_number}: unauthorized call target '{target}'")

for label in FUNCTIONS:
    if label not in found_labels:
        errors.append(f"missing function label: {label}")

clean = re.sub(r"#.*", "", text.lower())
if re.search(r"(?m)^\s*\.(?:data|bss|comm)\b", clean):
    errors.append("writable global storage is not allowed")

# Extract each public function body so required calls cannot be hidden elsewhere.
positions = []
for label in FUNCTIONS:
    match = re.search(rf"(?m)^\s*{re.escape(label)}\s*:\s*$", clean)
    if match:
        positions.append((match.start(), match.end(), label))
positions.sort()
bodies = {}
for index, (start, body_start, label) in enumerate(positions):
    body_end = positions[index + 1][0] if index + 1 < len(positions) else len(clean)
    bodies[label] = clean[body_start:body_end]

if "recursive_power" in bodies:
    recursive_calls = re.findall(r"\bcall\s+recursive_power\b", bodies["recursive_power"])
    if len(recursive_calls) != 1:
        errors.append("recursive_power must contain exactly one recursive call site")

if "combine_helpers" in bodies:
    square_calls = re.findall(r"\bcall\s+helper_square\b", bodies["combine_helpers"])
    affine_calls = re.findall(r"\bcall\s+helper_affine\b", bodies["combine_helpers"])
    if len(square_calls) != 1:
        errors.append("combine_helpers must contain exactly one helper_square call site")
    if len(affine_calls) != 1:
        errors.append("combine_helpers must contain exactly one helper_affine call site")

if errors:
    print("source-rule check failed:")
    for error in errors:
        print(f"  - {error}")
    sys.exit(1)

print("source-rule check passed")
