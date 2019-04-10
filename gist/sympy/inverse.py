import sympy as sy

def inverse(f,x,newxName:str):
    newx=sy.Symbol(newxName)
    s=sy.solve([sy.Eq(newx,f)],x)
    if isinstance(s,dict):
        return [s[x]],newx
    else:
        return s,newx
