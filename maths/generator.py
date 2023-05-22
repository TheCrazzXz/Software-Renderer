for x in range(4):
	for y in range(4):
		for z in range(4):
			print("m"+str(4 * x + z)+"p(matA) * m"+str(4 * z + y)+"p(matB)", end="")
			if z < 3:
				print(" + ", end="")
		print(",\n")

	print("\n")