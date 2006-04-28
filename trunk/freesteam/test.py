from freesteam import *
import unittest

class TestPythonBindings(unittest.TestCase):
    
	def testmeasurementpres(self):
		p = Measurement(10,"bar")
		self.assertEqual(p.value, 1e6)

	def testmeasurementtemp(self):
		T = Measurement(300,"K")
		self.assertEqual(T.value, 300)

	def testmeasurement_add(self):
		p = Measurement(7,"bar")
		p1 = Measurement(0.1,"bar")
		p2 = p + p1;
		self.assertAlmostEqual(p2,Measurement(7.1,"bar"))

	def testmeasurement_add2(self):
		p = Measurement(7,"bar")
		p1 = Measurement(0.1,"bar")
		p += p1;
		self.assertAlmostEqual(p,Measurement(7.1,"bar"))

	def testmeasurement_mul(self):
		L = Measurement(10,"m")
		W = Measurement(10,"cm")
		A = L * W;
		self.assertAlmostEqual(A,Measurement(1,"m2"))

	def testmeasurement_invalid_comparison(self):
		L = Measurement(10,"m")
		d = unittest.TestCase
		try:
			X = L - d
			self.fail("No error thrown!");
		except RuntimeError, e:
			pass
	
	def teststeam_ph(self):
		S = steam_ph()

	def teststeam_ph_sup(self):
		p = Measurement(10,"bar")
		h = Measurement(2998,"kJ/kg")
		try:
			S = steam_ph().solve(p,h)
			T = S.temp()
			self.assertAlmostEqual(T,Measurement(275+273.15,"K"),1)

		except RuntimeError, e:
			self.fail(e)
				
	def teststeam_ph_sub(self):
		p = Measurement(40,"bar")
		h = Measurement(422,"kJ/kg")
		S = steam_ph().solve(p,h)
		T = S.temp()
		self.assertAlmostEqual(T,Measurement(100+273.15,"K"),1)

	def teststeam_ph_sat(self):
		p = Measurement(10,"bar")
		h = Measurement(780,"kJ/kg")
		S = steam_ph().solve(p,h)
		T = S.temp()
		self.assertAlmostEqual(T,Measurement(179.9+273.15,"K"),1)

	def teststeam_ph_sat_2(self):
		p = Measurement(7,"bar")
		h = Measurement(2500,"kJ/kg")
		S = steam_ph().solve(p,h)
		T = S.temp()
		self.assertAlmostEqual(T,Measurement(165.0+273.15,"K"),1)

if __name__ == '__main__':
    unittest.main()
