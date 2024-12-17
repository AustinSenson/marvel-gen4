import os
import re

# Function to add a variable to a structure within a header file
def add_variable_to_structure(file_path, structure_name, new_variable):
    with open(file_path, 'r') as file:
        content = file.read()

    # Find the structure
    structure_start = content.find(f"typedef struct {{\n")
    structure_end = content.find(f"}} {structure_name};", structure_start)

    if structure_start == -1 or structure_end == -1:
        print(f"Structure {structure_name} not found in the file {file_path}.")
        return

    # Check if the variable already exists
    if new_variable.strip().split()[1] in content[structure_start:structure_end]:
        print(f"{new_variable.strip()} is already present in {structure_name} in {file_path}. No changes made.")
        return

    # Add the new variable to the structure if not present
    updated_content = content[:structure_end] + f"  {new_variable} /* Added variable */\n" + content[structure_end:]
    
    # Write updated content back
    with open(file_path, 'w') as file:
        file.write(updated_content)
    print(f"Added {new_variable} to {structure_name} in {file_path}.")

# Function to replace "* 100" with the replacement variable in `.c` files
def replace_duration_counter(c_file_path, replacement_variable):
    with open(c_file_path, 'r') as file:
        content = file.read()

    # Check if the replacement variable already exists
    if re.search(r"\* " + re.escape(replacement_variable), content):
        print(f"No changes needed in {c_file_path}. Already contains the replacement variable.")
        return

    # Replace occurrences
    pattern = r"(durationCounter_[a-zA-Z0-9_]+) \* 100"
    updated_content = re.sub(pattern, rf"\1 * {replacement_variable}", content)

    # Write updated content back
    with open(c_file_path, 'w') as file:
        file.write(updated_content)
    print(f"Replaced all occurrences of '* 100' with '* {replacement_variable}' for duration counters in {c_file_path}.")

# Function to append suffixes to specific functions within `.c` and `.h` files in a folder
def append_suffix_to_functions(folder_path, functions, suffix):
    function_regex = r'\b({})\b'.format('|'.join(re.escape(f) for f in functions))

    for root, _, files in os.walk(folder_path):
        for file_name in files:
            if file_name.endswith(('.c', '.h')):
                file_path = os.path.join(root, file_name)
                with open(file_path, 'r') as file:
                    file_content = file.read()

                # Replace function names with suffixes
                modified_content = re.sub(function_regex, lambda match: match.group(1) + suffix, file_content)
                
                with open(file_path, 'w') as file:
                    file.write(modified_content)
                print(f"Modified: {file_path}")

# Paths and configurations
base_path = "D:/Marvel_RTOS/marvel-rtos/Library/MATLAB/MARVEL_MBD_SUBMODULE"
files_to_modify = [
    (f"{base_path}/ChargingAlgorithm/ChargingAlgorithm.h", "ExtU_ChargingAlgorithm_T", "real_T looptimeCharging;"),
    (f"{base_path}/CellBalancing/CellBalancing.h", "ExtU_CellBalancing_T", "real_T looptimeCellBalancing;"),
    (f"{base_path}/Contactors/Contactors.h", "ExtU_Contactors_T", "real_T looptimeContactors;"),
    (f"{base_path}/CoulombCounting/SOCEstimation.h", "ExtU_SOCEstimation_T", "real_T looptimeSoC;"),
    (f"{base_path}/Protection/Protection.h", "ExtU_Protection_T", "real_T looptimeProtection;"),
    (f"{base_path}/StateMachine/HighLevelStatemachine.h", "ExtU_HighLevelStatemachine_T", "real_T looptimeStateMachine;")
]

file_replacements = {
    f"{base_path}/ChargingAlgorithm/ChargingAlgorithm.c": "ChargingAlgorithm_U.looptimeCharging",
    f"{base_path}/CellBalancing/CellBalancing.c": "CellBalancing_U.looptimeCellBalancing",
    f"{base_path}/Contactors/Contactors.c": "Contactors_U.looptimeContactors",
    f"{base_path}/CoulombCounting/SOCEstimation.c": "SOCEstimation_U.looptimeSoC",
    f"{base_path}/Protection/Protection.c": "Protection_U.looptimeProtection",
    f"{base_path}/StateMachine/HighLevelStatemachine.c": "HighLevelStatemachine_U.looptimeStateMachine",
}

folder_suffix_mapping = {
    "CellBalancing": "_cb",
    "ChargingAlgorithm": "_chg",
    "Contactors": "_c",
    "CoulombCounting": "_cc",
    "DataPipeline": "_dp",
    "Protection": "_p",
    "StateMachine": "_sm"
}

functions = ["mul_wide_s32", "mul_s32_sat", "rt_roundd_snf", "sMultiWordMul", "sMultiWordCmp", "sMultiWord2Single"]

# Executing the three functionalities in sequence
# 1. Add variables to structures
for file_path, structure_name, new_variable in files_to_modify:
    add_variable_to_structure(file_path, structure_name, new_variable)

# 2. Replace "* 100" with replacement variables
for c_file_path, replacement_variable in file_replacements.items():
    replace_duration_counter(c_file_path, replacement_variable)

# 3. Append suffixes to function names
for folder_name, suffix in folder_suffix_mapping.items():
    folder_path = os.path.join(base_path, folder_name)
    if os.path.exists(folder_path):
        print(f"Processing folder: {folder_name} with suffix: {suffix}")
        append_suffix_to_functions(folder_path, functions, suffix)
    else:
        print(f"Folder not found: {folder_path}")
