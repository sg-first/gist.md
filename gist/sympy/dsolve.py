import sympy

x=sympy.Symbol('x')
a=sympy.Symbol('a')
b=sympy.Symbol('b')
y=sympy.Function('y')

y=y(x)
y1=y.diff(x)
y2=y1.diff(x)
y3=y2.diff(x)


# result=sympy.dsolve(sympy.Eq(y2+(a**2)*y, b*sympy.sin(a)*y), y)
result=sympy.dsolve(sympy.Eq(y3+3*y2+3*y1+y, 1), y)
y=result.rhs
y1=y.diff(x)
y2=y1.diff(x)
y3=y2.diff(x)
print(y)
print(y1)
print(y2)

con1=sympy.Eq(y.subs(x,0), 0)
con2=sympy.Eq(y2.subs(x,0), 0)
con3=sympy.Eq(y1.subs(x,0), 0)
con4=sympy.Eq(y3.subs(x,0), 0)
print(con1)
print(con2)
print(con3)
print(con4)
constants = sympy.solve([con1,con2,con3], sympy.symbols('C1 C2 C3'))

print(constants)
print(y.subs(constants))