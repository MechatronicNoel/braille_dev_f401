import pywinusb.hid as hid


def sample_handler(data):
    print("Raw data: {0}".format(data))


# VID and PID customization changes here...
filter = hid.HidDeviceFilter(vendor_id = 0x483, product_id = 0x575A)
hid_device = filter.get_devices()
device = hid_device[0]
device.open()
print(hid_device)
target_usage = hid.get_full_usage_id(0x00, 0x3f)
device.set_raw_data_handler(sample_handler)
print(target_usage)

report = device.find_output_reports()
report_2 = device.find_input_reports()
print(report)
print(report_2)
print(report[0])

buffer = [0x8]*21
buffer[0] = 0
buffer[1] = 89


print(buffer)

report[0].set_raw_data(buffer)
report[0].send()