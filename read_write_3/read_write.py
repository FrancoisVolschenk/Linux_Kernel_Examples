print("writing to (1)")
with open("/dev/devDriver", "w") as fp:
	fp.write("Hi there")

print("reading back from (1)")
with open("/dev/devDriver", "r") as fp:
	line = fp.readline()
	print(line)

print("writing to (2)")
with open("/dev/devDriver", "w") as fp:
	fp.write("How are you?")

print("reading back from (2)")
with open("/dev/devDriver", "r") as fp:
	line = fp.readline()
	print(line)
