import csv
import struct
from tkinter import Tk, Label, Entry, Button, filedialog, Frame, Scrollbar, Canvas, LEFT, RIGHT, Y, BOTH, VERTICAL
from tkinter.filedialog import askopenfilename, asksaveasfilename
from PIL import Image, ImageTk

def search_config(event=None):  # Accept the event parameter
    search_term = entry_search.get().lower()
    for widget in frame.winfo_children():
        if isinstance(widget, Label):
            if search_term in widget.cget("text").lower():
                # Scroll to the widget using its absolute position within the canvas
                canvas.yview_moveto(widget.winfo_y() / frame.winfo_height())
                return  # Stop searching after the first match
            
def read_string_as_uint64_array(data_input):
    uint64_array = []
    
    # Convert to a string if the input is a list
    if isinstance(data_input, list):
        data_string = ','.join(str(item) for item in data_input)
    else:
        data_string = data_input
    
    # Split the string by commas
    values = data_string.split(',')
    
    # Parse each value as uint64 and combine the first two values
    combined_value = None
    for i, value in enumerate(values):
        try:
            # Convert the value to an integer
            int_value = int(value.strip())  # Assuming input values are in hex
            
            if i == 0:
                # Store the first value separately
                combined_value = int_value
            elif i == 1:
                # Combine the first and second values
                combined_value = (combined_value << 8) | int_value 
                combined_value <<= 8   # adding 0x00 to the last byte as minor config version is not being used    
            else:
                # Pack and unpack as uint64
                uint64_array.append(struct.unpack('<Q', struct.pack('<Q', int_value))[0])
        
        except ValueError:
            print(f"Invalid value: {value.strip()}")
    
    if combined_value is not None:
        uint64_array.insert(0, combined_value)
    
    return uint64_array

def write_csv_with_crc(data, crc_value):
    # Append CRC value to the data array
    data.append(crc_value)
    
    # Ask the user where to save the new CSV file
    save_path = asksaveasfilename(title="Save CSV with CRC", defaultextension=".csv", filetypes=[("CSV Files", "*.csv")])
    
    if save_path:
        # Write the updated data to the new CSV file
        with open(save_path, 'w', newline='') as file:
            csv_writer = csv.writer(file)
            # Write the data as a single row in the CSV
            csv_writer.writerow(data)
        print(f"Updated CSV saved to: {save_path}")
    else:
        print("No file selected for saving.")

# Function to calculate CRC
def calculate_flash_crc(data):
    poly = 0xEDB88320
    crc = 0xFFFFFFFF
    
    for value in data:
        crc ^= value
        for _ in range(32):
            mask = -(crc & 1)
            crc = (crc >> 1) ^ (poly & mask)
    
    return ~crc & 0xFFFFFFFF
# Function to save the config values to a CSV file
def save_config_values(values):
        
        data_array = read_string_as_uint64_array(values)

        # CRC calculation and appending can be added here
        crc_result = calculate_flash_crc(data_array)
        print(f"Calculated CRC: {crc_result}")

        # Step 3: Write the updated CSV with CRC as the last element
        write_csv_with_crc(data_array, crc_result)

        root.quit()  # Close the GUI after saving

# Function to get values from the GUI and save them
def save_configs():
    config_values = [
        entry_config_version_major.get(),
        entry_config_version_minor.get(),
        entry_number_of_cmu.get(),
        entry_cell_in_series.get(),
        entry_max_cell_per_slave.get(),
        entry_cell_in_slave_1.get(),
        entry_cell_in_slave_2.get(),
        entry_cell_in_slave_3.get(),
        entry_cell_in_slave_4.get(),
        entry_cell_in_slave_5.get(),
        entry_cell_in_slave_6.get(),
        entry_cell_in_slave_7.get(),
        entry_cell_in_slave_8.get(),
        entry_cell_in_slave_9.get(),
        entry_cell_in_slave_10.get(),
        entry_cell_in_slave_11.get(),
        entry_cell_in_slave_12.get(),
        entry_external_ntc_per_slave.get(),
        entry_internal_ntc_per_slave.get(),
        entry_ntc_beta_value.get(),
        entry_shunt_resistor.get(),
        entry_cell_max_voltage.get(),
        entry_cell_min_voltage.get(),
        entry_cell_balancing_start_voltage.get(),
        entry_cell_imbalance_threshold.get(),
        entry_max_charge_current.get(),
        entry_max_discharge_current.get(),
        entry_pack_max_capacity.get(),
        entry_pack_min_capacity.get(),
        entry_cell_max_temp.get(),
        entry_cell_min_temp.get(),
        entry_board_max_temp.get(),
        entry_board_min_temp.get(),
        entry_error_timeout.get(),
        entry_warning_timeout.get(),
        entry_recovery_timeout.get(),
        entry_max_allowed_temp_change.get(),
        entry_max_allowed_temp_gradient.get(),
        entry_high_imbalance_error.get(),
        entry_balancing_off_time.get(),
        entry_balancing_on_time.get(),
        entry_contactor_cut_off_time.get(),
        entry_pre_charge_current.get(),
        entry_precharge_retry_timeout.get(),
        entry_precharge_retry_limit.get(),
        entry_precharge_timeout.get(),
        entry_vehicle_rest_time.get(),
        entry_vehicle_rest_current.get(),
        entry_charge_current_detection_threshold.get(),
        entry_retry_priority_zero.get(),
        entry_retry_priority_one.get(),
        entry_retry_priority_two.get(),
        entry_retry_priority_three.get(),
        entry_retry_priority_four.get(),
        entry_retry_priority_five.get(),
        entry_balancing_derating_start_temp.get(),
        entry_balancing_derating_end_temp.get(),
        entry_balancing_max_on_time.get(),
        entry_balancing_min_on_time.get(),
        entry_balancing_max_off_time.get(),
        entry_balancing_min_off_time.get(),
        entry_occ_error_current.get(),
        entry_occ_warning_current.get(),
        entry_ocd_error_current.get(),
        entry_ocd_warning_current.get(),
        entry_otc_error_temp_group_1.get(),
        entry_otc_warning_temp_group_1.get(),
        entry_otc_recovery_temp_group_1.get(),
        entry_otd_error_temp_group_1.get(),
        entry_otd_warning_temp_group_1.get(),
        entry_otd_recovery_temp_group_1.get(),
        entry_utc_error_temp_group_1.get(),
        entry_utc_warning_temp_group_1.get(),
        entry_utc_recovery_temp_group_1.get(),
        entry_utd_error_temp_group_1.get(),
        entry_utd_warning_temp_group_1.get(),
        entry_utd_recovery_temp_group_1.get(),
        entry_arc_cartridge_temp.get(),
        entry_arc_cooling_temp.get(),
        entry_arc_cooling_temp_charging.get(),
        entry_arc_enable.get(),
        entry_arc_heating_temp.get(),
        entry_arc_heating_temp_charging.get(),
        entry_arc_cooling_type.get(),
    ]
    save_config_values(config_values)

# Function to load config values from a CSV file and populate the fields
def load_configs():
    file_path = filedialog.askopenfilename(defaultextension=".csv", filetypes=[("CSV files", "*.csv")])
    
    if file_path:
        with open(file_path, newline='') as file:
            csv_reader = csv.reader(file)
            config_values = next(csv_reader)
            entry_fields = [
                entry_config_version_major,
                entry_config_version_minor,
                entry_number_of_cmu,
                entry_cell_in_series,
                entry_max_cell_per_slave,
                entry_cell_in_slave_1,
                entry_cell_in_slave_2,
                entry_cell_in_slave_3,
                entry_cell_in_slave_4,
                entry_cell_in_slave_5,
                entry_cell_in_slave_6,
                entry_cell_in_slave_7,
                entry_cell_in_slave_8,
                entry_cell_in_slave_9,
                entry_cell_in_slave_10,
                entry_cell_in_slave_11,
                entry_cell_in_slave_12,
                entry_external_ntc_per_slave,
                entry_internal_ntc_per_slave,
                entry_ntc_beta_value,
                entry_shunt_resistor,
                entry_cell_max_voltage,
                entry_cell_min_voltage,
                entry_cell_balancing_start_voltage,
                entry_cell_imbalance_threshold,
                entry_max_charge_current,
                entry_max_discharge_current,
                entry_pack_max_capacity,
                entry_pack_min_capacity,
                entry_cell_max_temp,
                entry_cell_min_temp,
                entry_board_max_temp,
                entry_board_min_temp,
                entry_error_timeout,
                entry_warning_timeout,
                entry_recovery_timeout,
                entry_max_allowed_temp_change,
                entry_max_allowed_temp_gradient,
                entry_high_imbalance_error,
                entry_balancing_off_time,
                entry_balancing_on_time,
                entry_contactor_cut_off_time,
                entry_pre_charge_current,
                entry_precharge_retry_timeout,
                entry_precharge_retry_limit,
                entry_precharge_timeout,
                entry_vehicle_rest_time,
                entry_vehicle_rest_current,
                entry_charge_current_detection_threshold,
                entry_retry_priority_zero,
                entry_retry_priority_one,
                entry_retry_priority_two,
                entry_retry_priority_three,
                entry_retry_priority_four,
                entry_retry_priority_five,
                entry_balancing_derating_start_temp,
                entry_balancing_derating_end_temp,
                entry_balancing_max_on_time,
                entry_balancing_min_on_time,
                entry_balancing_max_off_time,
                entry_balancing_min_off_time,
                entry_occ_error_current,
                entry_occ_warning_current,
                entry_ocd_error_current,
                entry_ocd_warning_current,
                entry_otc_error_temp_group_1,
                entry_otc_warning_temp_group_1,
                entry_otc_recovery_temp_group_1,
                entry_otd_error_temp_group_1,
                entry_otd_warning_temp_group_1,
                entry_otd_recovery_temp_group_1,
                entry_utc_error_temp_group_1,
                entry_utc_warning_temp_group_1,
                entry_utc_recovery_temp_group_1,
                entry_utd_error_temp_group_1,
                entry_utd_warning_temp_group_1,
                entry_utd_recovery_temp_group_1,
                entry_otc_error_temp_group_2,
                entry_otc_warning_temp_group_2,
                entry_otc_recovery_temp_group_2,
                entry_otd_error_temp_group_2,
                entry_otd_warning_temp_group_2,
                entry_otd_recovery_temp_group_2,
                entry_utc_error_temp_group_2,
                entry_utc_warning_temp_group_2,
                entry_utc_recovery_temp_group_2,
                entry_utd_error_temp_group_2,
                entry_utd_warning_temp_group_2,
                entry_utd_recovery_temp_group_2,
                entry_otc_error_temp_group_3,
                entry_otc_warning_temp_group_3,
                entry_otc_recovery_temp_group_3,
                entry_otd_error_temp_group_3,
                entry_otd_warning_temp_group_3,
                entry_otd_recovery_temp_group_3,
                entry_utc_error_temp_group_3,
                entry_utc_warning_temp_group_3,
                entry_utc_recovery_temp_group_3,
                entry_utd_error_temp_group_3,
                entry_utd_warning_temp_group_3,
                entry_utd_recovery_temp_group_3,
                entry_otc_error_temp_group_4,
                entry_otc_warning_temp_group_4,
                entry_otc_recovery_temp_group_4,
                entry_otd_error_temp_group_4,
                entry_otd_warning_temp_group_4,
                entry_otd_recovery_temp_group_4,
                entry_utc_error_temp_group_4,
                entry_utc_warning_temp_group_4,
                entry_utc_recovery_temp_group_4,
                entry_utd_error_temp_group_4,
                entry_utd_warning_temp_group_4,
                entry_utd_recovery_temp_group_4,
                entry_otc_error_temp_group_5,
                entry_otc_warning_temp_group_5,
                entry_otc_recovery_temp_group_5,
                entry_otd_error_temp_group_5,
                entry_otd_warning_temp_group_5,
                entry_otd_recovery_temp_group_5,
                entry_utc_error_temp_group_5,
                entry_utc_warning_temp_group_5,
                entry_utc_recovery_temp_group_5,
                entry_utd_error_temp_group_5,
                entry_utd_warning_temp_group_5,
                entry_utd_recovery_temp_group_5,
                entry_arc_cartridge_temp,
                entry_arc_cooling_temp,
                entry_arc_cooling_temp_charging,
                entry_arc_enable,
                entry_arc_heating_temp,
                entry_arc_heating_temp_charging,
                entry_arc_cooling_type,
            ]
            
            # Populate the entry fields with the corresponding config values
            for i, entry in enumerate(entry_fields):
                entry.delete(0, 'end')
                entry.insert(0, config_values[i])

# Create the GUI window
root = Tk()
root.title("Config Parameter Input")

# Set the overall background color
root.configure(bg="black")

# Set the initial window size and make it resizable
root.geometry("750x700")
root.state('normal')

# Load the image and adjust its transparency
image = Image.open("background.png").convert("RGBA")
alpha = 200  # Set the transparency level (0 = fully transparent, 255 = fully opaque)
image.putalpha(alpha)

# Convert the image for Tkinter and create a label to display it
background_image = ImageTk.PhotoImage(image)
background_label = Label(root, image=background_image)
background_label.place(x=0, y=0, relwidth=1, relheight=1)

# Set the overall background color (as a fallback)
root.configure(bg="black")


# Create a heading label at the top
heading = Label(root, text="MARVEL CONFIGURATION UPDATE INTERFACE", font=("Arial", 16, "bold"), bg="black", fg="white")
heading.pack(pady=10)

# Add a search bar at the top
search_frame = Frame(root, bg="black")
search_frame.pack(pady=10)
Label(search_frame, text="Search Config:", font=("Arial", 12), bg="black", fg="white").pack(side=LEFT, padx=5)
entry_search = Entry(search_frame, width=30)
entry_search.pack(side=LEFT, padx=5)
Button(search_frame, text="Search", command=search_config).pack(side=LEFT, padx=5)

# Bind the Enter key to the search function
entry_search.bind("<Return>", search_config)

# Create a main frame to hold the canvas and scrollbar with padding
main_frame = Frame(root, bg="grey", padx=20, pady=20)
main_frame.pack(fill=BOTH, expand=1)

# Create a canvas inside the main frame
canvas = Canvas(main_frame, bg="lightgrey", highlightthickness=0)
canvas.pack(side=LEFT, fill=BOTH, expand=1)

# Add a scrollbar to the canvas
scrollbar = Scrollbar(main_frame, orient=VERTICAL, command=canvas.yview)
scrollbar.pack(side=RIGHT, fill=Y)

# Configure the canvas to use the scrollbar
canvas.configure(yscrollcommand=scrollbar.set)
canvas.bind('<Configure>', lambda e: canvas.configure(scrollregion=canvas.bbox("all")))

# Create another frame inside the canvas
frame = Frame(canvas, bg="lightgrey")
canvas.create_window((0, 0), window=frame, anchor="nw")

# Function to create label and entry fields in a grid
def create_label_entry(parent, label_text, row):
    label = Label(parent, text=label_text, padx=10, pady=5, bg="lightgrey", fg="black", font=("Arial", 10, "bold"))
    label.grid(row=row, column=0, sticky="w", padx=10, pady=5)
    entry = Entry(parent, width=50)
    entry.grid(row=row, column=1, padx=10, pady=5)
    return entry

# Enable mouse scrolling on Windows and Linux
def on_mouse_wheel(event):
    canvas.yview_scroll(int(-1*(event.delta/120)), "units")

# Enable mouse scrolling on macOS (delta is different)
def on_mouse_wheel_mac(event):
    canvas.yview_scroll(-1 if event.delta > 0 else 1, "units")

# Bind the mouse scroll event globally to the root window for cross-GUI scrolling
if root.tk.call("tk", "windowingsystem") == "aqua":  # macOS
    root.bind_all("<MouseWheel>", on_mouse_wheel_mac)
else:  # Windows and Linux
    root.bind_all("<MouseWheel>", on_mouse_wheel)

# Define labels and entries in the frame
entry_config_version_major = create_label_entry(frame, "Config Version MAJOR:", 0)
entry_config_version_minor = create_label_entry(frame, "Config Version MINOR:", 1)
entry_number_of_cmu = create_label_entry(frame, "Number of CMU:", 2)
entry_cell_in_series = create_label_entry(frame, "Cell In Series:", 3)
entry_max_cell_per_slave = create_label_entry(frame, "Max Cell Per Slave:", 4)
entry_cell_in_slave_1 = create_label_entry(frame, "Cell In Slave 1:", 5)
entry_cell_in_slave_2 = create_label_entry(frame, "Cell In Slave 2:", 6)
entry_cell_in_slave_3 = create_label_entry(frame, "Cell In Slave 3:", 7)
entry_cell_in_slave_4 = create_label_entry(frame, "Cell In Slave 4:", 8)
entry_cell_in_slave_5 = create_label_entry(frame, "Cell In Slave 5:", 9)
entry_cell_in_slave_6 = create_label_entry(frame, "Cell In Slave 6:", 10)
entry_cell_in_slave_7 = create_label_entry(frame, "Cell In Slave 7:", 11)
entry_cell_in_slave_8 = create_label_entry(frame, "Cell In Slave 8:", 12)
entry_cell_in_slave_9 = create_label_entry(frame, "Cell In Slave 9:", 13)
entry_cell_in_slave_10 = create_label_entry(frame, "Cell In Slave 10:", 14)
entry_cell_in_slave_11 = create_label_entry(frame, "Cell In Slave 11:", 15)
entry_cell_in_slave_12 = create_label_entry(frame, "Cell In Slave 12:", 16)
entry_external_ntc_per_slave = create_label_entry(frame, "External NTC Per Slave:", 17)
entry_internal_ntc_per_slave = create_label_entry(frame, "Internal NTC Per Slave:", 18)
entry_ntc_beta_value = create_label_entry(frame, "NTC Beta Value:", 19)
entry_shunt_resistor = create_label_entry(frame, "Shunt Resistor:", 20)
entry_cell_max_voltage = create_label_entry(frame, "Cell Max Voltage:", 21)
entry_cell_min_voltage = create_label_entry(frame, "Cell Min Voltage:", 22)
entry_cell_balancing_start_voltage = create_label_entry(frame, "Cell Balancing Start Voltage:", 23)
entry_cell_imbalance_threshold = create_label_entry(frame, "Cell Imbalance Threshold:", 24)
entry_max_charge_current = create_label_entry(frame, "Max Charge Current:", 25)
entry_max_discharge_current = create_label_entry(frame, "Max Discharge Current:", 26)
entry_pack_max_capacity = create_label_entry(frame, "Pack Max Capacity:", 27)
entry_pack_min_capacity = create_label_entry(frame, "Pack Min Capacity:", 28)
entry_cell_max_temp = create_label_entry(frame, "Cell Max Temp:", 29)
entry_cell_min_temp = create_label_entry(frame, "Cell Min Temp:", 30)
entry_board_max_temp = create_label_entry(frame, "Board Max Temp:", 31)
entry_board_min_temp = create_label_entry(frame, "Board Min Temp:", 32)
entry_error_timeout = create_label_entry(frame, "Error Timeout:", 33)
entry_warning_timeout = create_label_entry(frame, "Warning Timeout:", 34)
entry_recovery_timeout = create_label_entry(frame, "Recovery Timeout:", 35)
entry_max_allowed_temp_change = create_label_entry(frame, "Max Allowed Temp Change:", 36)
entry_max_allowed_temp_gradient = create_label_entry(frame, "Max Allowed Temp Gradient:", 37)
entry_high_imbalance_error = create_label_entry(frame, "High Imbalance Error:", 38)
entry_balancing_off_time = create_label_entry(frame, "Balancing Off Time:", 39)
entry_balancing_on_time = create_label_entry(frame, "Balancing On Time:", 40)
entry_contactor_cut_off_time = create_label_entry(frame, "Contactor Cut Off Time:", 41)
entry_pre_charge_current = create_label_entry(frame, "Pre-charge Current:", 42)
entry_precharge_retry_timeout = create_label_entry(frame, "Pre-charge Retry Timeout:", 43)
entry_precharge_retry_limit = create_label_entry(frame, "Pre-charge Retry Limit:", 44)
entry_precharge_timeout = create_label_entry(frame, "Pre-charge Timeout:", 45)
entry_vehicle_rest_time = create_label_entry(frame, "Vehicle Rest Time:", 46)
entry_vehicle_rest_current = create_label_entry(frame, "Vehicle Rest Current:", 47)
entry_charge_current_detection_threshold = create_label_entry(frame, "Charge Current Detection Threshold:", 48)
entry_retry_priority_zero = create_label_entry(frame, "Retry Priority Zero:", 49)
entry_retry_priority_one = create_label_entry(frame, "Retry Priority One:", 50)
entry_retry_priority_two = create_label_entry(frame, "Retry Priority Two:", 51)
entry_retry_priority_three = create_label_entry(frame, "Retry Priority Three:", 52)
entry_retry_priority_four = create_label_entry(frame, "Retry Priority Four:", 53)
entry_retry_priority_five = create_label_entry(frame, "Retry Priority Five:", 54)
entry_balancing_derating_start_temp = create_label_entry(frame, "Balancing Derating Start Temp:", 55)
entry_balancing_derating_end_temp = create_label_entry(frame, "Balancing Derating End Temp:", 56)
entry_balancing_max_on_time = create_label_entry(frame, "Balancing Max On Time:", 57)
entry_balancing_min_on_time = create_label_entry(frame, "Balancing Min On Time:", 58)
entry_balancing_max_off_time = create_label_entry(frame, "Balancing Max Off Time:", 59)
entry_balancing_min_off_time = create_label_entry(frame, "Balancing Min Off Time:", 60)
entry_occ_error_current = create_label_entry(frame, "OCC Error Current:", 61)
entry_occ_warning_current = create_label_entry(frame, "OCC Warning Current:", 62)
entry_ocd_error_current = create_label_entry(frame, "OCD Error Current:", 63)
entry_ocd_warning_current = create_label_entry(frame, "OCD Warning Current:", 64)

# Group 1
entry_otc_error_temp_group_1 = create_label_entry(frame, "OTC Error Temp Group 1:", 65)
entry_otc_warning_temp_group_1 = create_label_entry(frame, "OTC Warning Temp Group 1:", 66)
entry_otc_recovery_temp_group_1 = create_label_entry(frame, "OTC Recovery Temp Group 1:", 67)
entry_otd_error_temp_group_1 = create_label_entry(frame, "OTD Error Temp Group 1:", 68)
entry_otd_warning_temp_group_1 = create_label_entry(frame, "OTD Warning Temp Group 1:", 69)
entry_otd_recovery_temp_group_1 = create_label_entry(frame, "OTD Recovery Temp Group 1:", 70)
entry_utc_error_temp_group_1 = create_label_entry(frame, "UTC Error Temp Group 1:", 71)
entry_utc_warning_temp_group_1 = create_label_entry(frame, "UTC Warning Temp Group 1:", 72)
entry_utc_recovery_temp_group_1 = create_label_entry(frame, "UTC Recovery Temp Group 1:", 73)
entry_utd_error_temp_group_1 = create_label_entry(frame, "UTD Error Temp Group 1:", 74)
entry_utd_warning_temp_group_1 = create_label_entry(frame, "UTD Warning Temp Group 1:", 75)
entry_utd_recovery_temp_group_1 = create_label_entry(frame, "UTD Recovery Temp Group 1:", 76)

# Group 2
entry_otc_error_temp_group_2 = create_label_entry(frame, "OTC Error Temp Group 2:", 77)
entry_otc_warning_temp_group_2 = create_label_entry(frame, "OTC Warning Temp Group 2:", 78)
entry_otc_recovery_temp_group_2 = create_label_entry(frame, "OTC Recovery Temp Group 2:", 79)
entry_otd_error_temp_group_2 = create_label_entry(frame, "OTD Error Temp Group 2:", 80)
entry_otd_warning_temp_group_2 = create_label_entry(frame, "OTD Warning Temp Group 2:", 81)
entry_otd_recovery_temp_group_2 = create_label_entry(frame, "OTD Recovery Temp Group 2:", 82)
entry_utc_error_temp_group_2 = create_label_entry(frame, "UTC Error Temp Group 2:", 83)
entry_utc_warning_temp_group_2 = create_label_entry(frame, "UTC Warning Temp Group 2:", 84)
entry_utc_recovery_temp_group_2 = create_label_entry(frame, "UTC Recovery Temp Group 2:", 85)
entry_utd_error_temp_group_2 = create_label_entry(frame, "UTD Error Temp Group 2:", 86)
entry_utd_warning_temp_group_2 = create_label_entry(frame, "UTD Warning Temp Group 2:", 87)
entry_utd_recovery_temp_group_2 = create_label_entry(frame, "UTD Recovery Temp Group 2:", 88)

# Group 3
entry_otc_error_temp_group_3 = create_label_entry(frame, "OTC Error Temp Group 3:", 89)
entry_otc_warning_temp_group_3 = create_label_entry(frame, "OTC Warning Temp Group 3:", 90)
entry_otc_recovery_temp_group_3 = create_label_entry(frame, "OTC Recovery Temp Group 3:", 91)
entry_otd_error_temp_group_3 = create_label_entry(frame, "OTD Error Temp Group 3:", 92)
entry_otd_warning_temp_group_3 = create_label_entry(frame, "OTD Warning Temp Group 3:", 93)
entry_otd_recovery_temp_group_3 = create_label_entry(frame, "OTD Recovery Temp Group 3:", 94)
entry_utc_error_temp_group_3 = create_label_entry(frame, "UTC Error Temp Group 3:", 95)
entry_utc_warning_temp_group_3 = create_label_entry(frame, "UTC Warning Temp Group 3:", 96)
entry_utc_recovery_temp_group_3 = create_label_entry(frame, "UTC Recovery Temp Group 3:", 97)
entry_utd_error_temp_group_3 = create_label_entry(frame, "UTD Error Temp Group 3:", 98)
entry_utd_warning_temp_group_3 = create_label_entry(frame, "UTD Warning Temp Group 3:", 99)
entry_utd_recovery_temp_group_3 = create_label_entry(frame, "UTD Recovery Temp Group 3:", 100)

# Group 4
entry_otc_error_temp_group_4 = create_label_entry(frame, "OTC Error Temp Group 4:", 101)
entry_otc_warning_temp_group_4 = create_label_entry(frame, "OTC Warning Temp Group 4:", 102)
entry_otc_recovery_temp_group_4 = create_label_entry(frame, "OTC Recovery Temp Group 4:", 103)
entry_otd_error_temp_group_4 = create_label_entry(frame, "OTD Error Temp Group 4:", 104)
entry_otd_warning_temp_group_4 = create_label_entry(frame, "OTD Warning Temp Group 4:", 105)
entry_otd_recovery_temp_group_4 = create_label_entry(frame, "OTD Recovery Temp Group 4:", 106)
entry_utc_error_temp_group_4 = create_label_entry(frame, "UTC Error Temp Group 4:", 107)
entry_utc_warning_temp_group_4 = create_label_entry(frame, "UTC Warning Temp Group 4:", 108)
entry_utc_recovery_temp_group_4 = create_label_entry(frame, "UTC Recovery Temp Group 4:", 109)
entry_utd_error_temp_group_4 = create_label_entry(frame, "UTD Error Temp Group 4:", 110)
entry_utd_warning_temp_group_4 = create_label_entry(frame, "UTD Warning Temp Group 4:", 111)
entry_utd_recovery_temp_group_4 = create_label_entry(frame, "UTD Recovery Temp Group 4:", 112)

# Group 5
entry_otc_error_temp_group_5 = create_label_entry(frame, "OTC Error Temp Group 5:", 113)
entry_otc_warning_temp_group_5 = create_label_entry(frame, "OTC Warning Temp Group 5:", 114)
entry_otc_recovery_temp_group_5 = create_label_entry(frame, "OTC Recovery Temp Group 5:", 115)
entry_otd_error_temp_group_5 = create_label_entry(frame, "OTD Error Temp Group 5:", 116)
entry_otd_warning_temp_group_5 = create_label_entry(frame, "OTD Warning Temp Group 5:", 117)
entry_otd_recovery_temp_group_5 = create_label_entry(frame, "OTD Recovery Temp Group 5:", 118)
entry_utc_error_temp_group_5 = create_label_entry(frame, "UTC Error Temp Group 5:", 119)
entry_utc_warning_temp_group_5 = create_label_entry(frame, "UTC Warning Temp Group 5:", 120)
entry_utc_recovery_temp_group_5 = create_label_entry(frame, "UTC Recovery Temp Group 5:", 121)
entry_utd_error_temp_group_5 = create_label_entry(frame, "UTD Error Temp Group 5:", 122)
entry_utd_warning_temp_group_5 = create_label_entry(frame, "UTD Warning Temp Group 5:", 123)
entry_utd_recovery_temp_group_5 = create_label_entry(frame, "UTD Recovery Temp Group 5:", 124)

entry_arc_cartridge_temp = create_label_entry(frame, "ARC Cartridge Temp:", 125)
entry_arc_cooling_temp = create_label_entry(frame, "ARC Cooling Temp:", 126)
entry_arc_cooling_temp_charging = create_label_entry(frame, "ARC Cooling Temp Charging:", 127)
entry_arc_enable = create_label_entry(frame, "ARC Enable:", 128)
entry_arc_heating_temp = create_label_entry(frame, "ARC Heating Temp:", 129)
entry_arc_heating_temp_charging = create_label_entry(frame, "ARC Heating Temp Charging:", 130)
entry_arc_cooling_type = create_label_entry(frame, "ARC Cooling Type:", 131)

# Add buttons for saving and loading the configurations
button_save = Button(root, text="Save Configs", command=save_configs)
button_save.pack(pady=10)

button_load = Button(root, text="Load Configs", command=load_configs)
button_load.pack(pady=10)

# Run the GUI loop
root.mainloop()
