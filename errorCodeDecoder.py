import tkinter as tk
from tkinter import ttk

def get_errors_from_code(error_code):
    # Define the error mapping
    error_matrix = {
        1: {
            1: "Over Current Charge",
            2: "High Imbalance",
            4: "PCB Temperature Error",
            8: "External NTC Temperature Error",
            16: "Efuse Discharge",
            32: "Efuse Charge",
            64: "Under Voltage",
            128: "Over Voltage",
        },
        0: {
            1: "Over Current Discharge",
            4: "EEPROM Write Fail",
            8: "EEPROM Read Fail",
            16: "Marvel Permanent Fail",  # Corrected bit value
            32: "Precharge Retry Fail",   # Corrected bit value
        },
    }

    errors = []
    # Check each byte and extract errors
    for byte, error_map in error_matrix.items():
        byte_value = (error_code >> (byte * 8)) & 0xFF  # Extract the relevant byte
        for bit_value, error_desc in error_map.items():
            if byte_value & bit_value:  # Check if the bit is set
                errors.append(error_desc)

    return errors

def decode_errors():
    try:
        # Clear previous results
        result_text.config(state=tk.NORMAL)  # Enable editing to clear content
        result_text.delete(1.0, tk.END)  # Clear previous results

        error_code = int(entry_error_code.get())  # Get input from the user
        errors = get_errors_from_code(error_code)
        if errors:
            result_text.insert(tk.END, f"Number of errors detected: {len(errors)}\n\n")
            result_text.insert(tk.END, "Errors present in the system:\n")
            for error in errors:
                result_text.insert(tk.END, f"- {error}\n")
        else:
            result_text.insert(tk.END, "Number of errors detected: 0\n\n")
            result_text.insert(tk.END, "No errors detected.")

        result_text.config(state=tk.DISABLED)  # Disable editing of the result box
    except ValueError:
        result_text.config(state=tk.NORMAL)  # Enable editing to display the error
        result_text.delete(1.0, tk.END)  # Clear previous results
        result_text.insert(tk.END, "Invalid input! Please enter a valid integer error code.")
        result_text.config(state=tk.DISABLED)  # Disable editing of the result box

# Create the GUI window
root = tk.Tk()
root.title("MARVEL Code Decoder")
root.geometry("500x400")
root.config(bg="#e8f4f8")

# Input section frame
frame_input = tk.Frame(root, bg="#d4e6f1", bd=2, relief=tk.GROOVE)
frame_input.pack(pady=10, padx=10, fill=tk.X)

label = tk.Label(frame_input, text="Enter Error Code (Decimal):", font=("Arial", 12, "bold"), bg="#d4e6f1")
label.pack(pady=5, anchor="w")

entry_error_code = tk.Entry(frame_input, font=("Arial", 12), width=20)
entry_error_code.pack(pady=5)

button_decode = tk.Button(frame_input, text="Decode Errors", command=decode_errors, font=("Arial", 12, "bold"), bg="#5dade2", fg="white")
button_decode.pack(pady=10)

# Results section frame
frame_results = tk.Frame(root, bg="#d4e6f1", bd=2, relief=tk.GROOVE)
frame_results.pack(pady=10, padx=10, fill=tk.BOTH, expand=True)

label_results = tk.Label(frame_results, text="Decoded Errors:", font=("Arial", 12, "bold"), bg="#d4e6f1")
label_results.pack(pady=5, anchor="w")

# Scrollbar
scrollbar = ttk.Scrollbar(frame_results, orient=tk.VERTICAL)

# Results display with scrollbar attached
result_text = tk.Text(
    frame_results,
    font=("Arial", 12),
    height=10,
    width=50,
    state=tk.DISABLED,
    bg="#f8f9f9",
    yscrollcommand=scrollbar.set,
)
scrollbar.config(command=result_text.yview)
scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
result_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

# Enable mouse wheel scrolling
def on_mouse_wheel(event):
    result_text.yview_scroll(-1 * int(event.delta / 120), "units")

result_text.bind("<MouseWheel>", on_mouse_wheel)  # Windows and MacOS
result_text.bind("<Button-4>", lambda e: result_text.yview_scroll(-1, "units"))  # Linux scroll up
result_text.bind("<Button-5>", lambda e: result_text.yview_scroll(1, "units"))  # Linux scroll down

# Run the GUI loop
root.mainloop()
