l = range(127, 152)

def divby(l, n):
	newL = []
	for i in l:
		if i % n == 0:
			newL.append(i)
	return newL

divBy7 = divby(l, 7)
print "Values divisibly by 7:"
print "\t", map(hex, divBy7)
print "\tTheir sum =", hex(sum(divBy7))

divBy3 = divby(l, 3)
print "Values divisibly by 3:"
print "\t", map(hex, divBy3)
print "\tTheir sum =", hex(sum(divBy3))