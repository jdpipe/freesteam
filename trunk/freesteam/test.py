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

	def testmeasurement_sub(self):
		p = Measurement(7,"bar")
		p1 = Measurement(0.1,"bar")
		p2 = p - p1;
		self.assertAlmostEqual(p2,Measurement(6.9,"bar"))

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

	def testmeasurement_algebra(self):
		m = Measurement(1,"g")
		t = Measurement(1,"s")
		l = Measurement(1,"m");
		mu = m/t/l;
		self.assertAlmostEqual(mu,Measurement(1,"g/sm"))
	
	def testmeasurement_power(self):
		K = Measurement(1,"K");
		K2 = K**2;
		K3 = K2/K;
		self.assertAlmostEqual(K3,Measurement(1,"K"))

	# NOW FOR THE MEAT IN YOUR SANDWICH
	
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

	def teststeam_pT(self):
		S = steam_pT().solve(Measurement(40,"bar"),Measurement(250+273.15,"K"));
		self.assertAlmostEqual(S.dens(),Measurement(799,"kg/m3"),0)

	# SATURATION

	def teststeam_sat_liq(self):
		p = Measurement(10,"bar");
		S = steam(); S.setSatWater_p(p)
		T = S.temp()
		rho = S.dens()
		self.assertAlmostEqual(T,Measurement(179.9+273.15,"K"),1)
		self.assertAlmostEqual(S.specenthalpy(),Measurement(762.65,"kJ/kg"),-2)
		self.assertAlmostEqual(S.dens(),Measurement(1/0.001127,"kg/m3"),0)
		self.assertAlmostEqual(S.specienergy(),Measurement(761.5,"kJ/kg"),-3)
		self.assertAlmostEqual(S.specentropy(),Measurement(2.138,"kJ/kgK"),0)

	def teststeam_sat_gas(self):
		p = Measurement(2,"bar");
		S = steam(); S.setSatSteam_p(p)
		T = S.temp()
		self.assertAlmostEqual(T,Measurement(120.2+273.15,"K"),1)
		self.assertAlmostEqual(S.specenthalpy(),Measurement(2706.3,"kJ/kg"),-3)
		self.assertAlmostEqual(S.specentropy(),Measurement(7.127,"kJ/kgK"),0)

	def teststeam_sat_gas_2(self):
		T = Measurement(111.4+273.15,"K")
		S = steam(); S.setSatSteam_T(T)
		p = S.pres()
		self.assertAlmostEqual(p,Measurement(0.15,"MPa"),-5)
		self.assertAlmostEqual(S.specenthalpy(),Measurement(2693.4,"kJ/kg"),-3)
		self.assertAlmostEqual(S.specentropy(),Measurement(7.223,"kJ/kgK"),-1)

	def teststeam_sat_gas_3(self):
		p = Measurement(150,"bar")
		S = steam(); S.setSatSteam_p(p)
		T = S.temp()
		self.assertAlmostEqual(T,Measurement(342.1+273.15,"K"),0)
		self.assertAlmostEqual(S.specenthalpy(),Measurement(2615.0,"kJ/kg"),-4)
		self.assertAlmostEqual(S.specentropy(),Measurement(5.318,"kJ/kgK"),-2)

	# CONDUCTIVITY

	def testconductivity_1(self):
		p = Measurement(1,"MPa")
		T = Measurement(350+273.15,"K")
		S = steam(); S.set_pT(p,T)
		k = S.conductivity()
		self.assertAlmostEqual(k,Measurement(49.8,"mW/mK"),4)

	def testconductivity_2(self):
		p = Measurement(5,"MPa")
		T = Measurement(25+273.15,"K")
		S = steam(); S.set_pT(p,T)
		k = S.conductivity()
		self.assertAlmostEqual(k,Measurement(610.1,"mW/mK"),4)

	def testconductivity_3(self):
		p = Measurement(1,"MPa")
		T = Measurement(600+273.15,"K")
		S = steam(); S.set_pT(p,T)
		k = S.conductivity()
		self.assertAlmostEqual(k,Measurement(80.4,"mW/mK"),4)

	def testconductivity_4(self):
		p = Measurement(0.5,"MPa")
		T = Measurement(200+273.15,"K")
		S = steam(); S.set_pT(p,T)
		k = S.conductivity()
		rho = S.dens()
		self.assertAlmostEqual(k,Measurement(34.2,"mW/mK"),4)

	def testconductivity_5(self):
		p = Measurement(0.5,"MPa")
		T = Measurement(750+273.15,"K")
		S = steam(); S.set_pT(p,T)
		k = S.conductivity()
		rho = S.dens()
		self.assertAlmostEqual(k,Measurement(100.8,"mW/mK"),4)

	def testconductivity_6(self):
		p = Measurement(100,"MPa")
		T = Measurement(800+273.15,"K")
		S = steam(); S.set_pT(p,T)
		k = S.conductivity()
		rho = S.dens()
		self.assertAlmostEqual(k,Measurement(213.2,"mW/mK"),4)

	# PICKLING / UNPICKLING

	def testpickle_measurement(self):
		#print "MEASUREMENT PICKLING"
		from cPickle import load,dump
		import os
		m = Measurement(150,"bar")
		#print "PICKLING m =",m
		fn = "temp-pickle-measurement.pkl"
		f = file(fn,"w");
		dump(m,f);
		f = file(fn,"r");
		m2 = load(f)
		#print "UNPICKLED m2 = ",m2
		self.assertAlmostEqual(m,m2,8)
		m = m + Measurement(1,"bar")
		self.assertNotAlmostEqual(m,m2,8)
		m3 = Measurement(m.value,"m")
		self.assertNotEqual(m3.dim,m2.dim)
		os.unlink(fn)

	def testpickle_steam(self):
		#print "STEAM PICKLING!"
		from cPickle import load,dump
		import os
		self.steampickle(Measurement(30,"bar"),Measurement(2000,"kJ/kg"))
		self.steampickle(Measurement(10,"bar"),Measurement(2000,"kJ/kg"))
		self.steampickle(Measurement(400,"bar"),Measurement(2000,"kJ/kg"))
		self.steampickle(Measurement(50,"bar"),Measurement(200,"kJ/kg"))
		self.steampickle(Measurement(60,"bar"),Measurement(4000,"kJ/kg"))
		
	def steampickle(self,p,h):
		from cPickle import load,dump
		import os
		S = steam_ph().solve(p,h)
		#print "Pickling region ",S.whichRegion()
		fn = "temp-pickle-steam.pkl"
		f = file(fn,"w")
		dump(S,f)
		del f
		#print "PICKLED"
		f1 = file(fn,"r")
		S2 = load(f1)
		#print "UNPICKLED"
		#print "p = ",S2.pres(),", h = ",S2.specenthalpy()
		#print "dp = ",(S2.pres() - S.pres())
		self.assertAlmostEqual(S2.pres(),S.pres())
		self.assertAlmostEqual(S2.specenthalpy(),S.specenthalpy())
		os.unlink(fn)
		

if __name__ == '__main__':
    unittest.main()
