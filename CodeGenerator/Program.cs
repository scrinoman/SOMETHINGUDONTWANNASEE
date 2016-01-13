using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Reflection.Emit;
using System.Globalization;
using System.IO;

namespace CodeGenerator
{
    class Program
    {
        static List<LocalBuilder> curLocals = new List<LocalBuilder>();
        static List<Type> curTypes = new List<Type>();
        static List<int> curScopes = new List<int>();
        static List<MethodBuilder> methods = new List<MethodBuilder>();
        static List<int> methodParamCount = new List<int>();
        static int curScope = 0;
        static MethodBuilder builder = null;

        static string[] Split(string s)
        {
            List<string> res = new List<string>();
            bool isInChar = false, isInString = false;
            string cur = "";
            for (int i = 0; i < s.Length; ++i)
            {
                if (s[i] == ' ' && !isInChar && !isInString)
                {
                    if (cur != "")
                    {
                        res.Add(cur);
                        cur = "";
                    }
                }
                else
                {
                    if (s[i] == '\'' && !isInString)
                    {
                        isInChar = !isInChar;
                    }

                    if (s[i] == '\"' && !isInChar)
                    {
                        isInString = !isInString;
                    }

                    cur += s[i];
                }
            }
            if (cur != "")
            {
                res.Add(cur);
            }

            return res.ToArray();
        }

        static char StringToChar(string s)
        {
            if (s.Length == 3)
            {
                return Convert.ToChar(s[1]);
            }
            else
            {
                //s[1] == '\\'
                switch (s[2])
                {
                    case 'n': return '\n';
                    case 't': return '\t';
                    case 'r': return '\r';
                    case 'a': return '\a';
                    case 'b': return '\b';
                    case 'f': return '\f';
                    case 'v': return '\v';
                    case '\'':
                    case '\"':
                    case '\\': return s[2];    
                    default: return '\0';
                }
            }
        }

        static OpCode GetOpCodeForOperator(string op)
        {
            if (op == "+") return OpCodes.Add;
            if (op == "-") return OpCodes.Sub;
            if (op == "*") return OpCodes.Mul;
            if (op == "/") return OpCodes.Div;
            if (op == "%") return OpCodes.Rem;
            if (op == "|") return OpCodes.Or;
            if (op == "&") return OpCodes.And;
            if (op == "unary_minus") return OpCodes.Neg;
            if (op == ">") return OpCodes.Cgt;
            if (op == "<") return OpCodes.Clt;
            if (op == ">=") 
            {
                builder.GetILGenerator().Emit(OpCodes.Clt);
                builder.GetILGenerator().Emit(OpCodes.Ldc_I4_1);
                builder.GetILGenerator().Emit(OpCodes.Sub);
                return OpCodes.Neg; 
            }
            if (op == "<=")
            {
                builder.GetILGenerator().Emit(OpCodes.Cgt);
                builder.GetILGenerator().Emit(OpCodes.Ldc_I4_1);
                builder.GetILGenerator().Emit(OpCodes.Sub);
                return OpCodes.Neg; 
            }
            if (op == "==") return OpCodes.Ceq;
            if (op == "!=") 
            {
                builder.GetILGenerator().Emit(OpCodes.Ceq);
                builder.GetILGenerator().Emit(OpCodes.Ldc_I4_1);
                builder.GetILGenerator().Emit(OpCodes.Sub);
                return OpCodes.Neg; 
            };
            if (op == "&&") return OpCodes.And;
            if (op == "||") return OpCodes.Or;
            if (op == "!")
            {
                builder.GetILGenerator().Emit(OpCodes.Ldc_I4_1);
                builder.GetILGenerator().Emit(OpCodes.Sub);
                return OpCodes.Neg; 
            }

            return OpCodes.Nop;
        }

        static Type GetTypeByString(string type, bool arrayType = false, bool array2D = false)
        {
            if (type == "int") return arrayType ? array2D ? typeof(int[,]) : typeof(int[]) : typeof(int);
            if (type == "float") return arrayType ? array2D ? typeof(float[,]) : typeof(float[]) : typeof(float);
            if (type == "string") return arrayType ? array2D ? typeof(string[,]) : typeof(string[]) : typeof(string);
            if (type == "char") return arrayType ? array2D ? typeof(char[,]) : typeof(char[]) : typeof(char);
            
            return typeof(void);
        }

        static string GetConvertMethodName(Type type)
        {
            if (type == typeof(int)) return "ToInt32";
            if (type == typeof(float)) return "ToSingle";
            if (type == typeof(string)) return "ToString";
            if (type == typeof(char)) return "ToChar";

            return "ToVoid";
        }

        static bool IsType(string type)
        {
            return (type == "CONST_INTEGER" || type == "CONST_FLOAT" || type == "CONST_CHARACTER" || type == "CONST_STRING");
        }

        static MethodBuilder CreateNewMethod(TypeBuilder builder, string methodName, Type returnType, Type[] allParams)
        {
            return builder.DefineMethod(methodName, MethodAttributes.HideBySig | MethodAttributes.Private | MethodAttributes.Static,
                returnType, allParams);
        }
        
        static Type LoadVariable(ILGenerator gen, string[] commands, ref int pointer)
        {
            pointer++;
            int dim = Convert.ToInt32(commands[pointer++].ToString());
            int varPointer = Convert.ToInt32(commands[pointer++].ToString());
            string strType = commands[pointer++];
            Type type = GetTypeByString(strType);
            int curVarPointer = varPointer - methodParamCount[methodParamCount.Count - 1];
            if (dim == 0)
            {
                if (curVarPointer >= 0)
                {
                    gen.Emit(OpCodes.Stloc, curLocals[curVarPointer]);
                }
                else
                {
                    gen.Emit(OpCodes.Starg, varPointer);
                }
            }
            else
            {
                if (dim == 1)
                {
                    gen.Emit(OpCodes.Ldloc, curLocals[curVarPointer]);
                    pointer++;
                    CreateExpression(gen, commands, ref pointer);
                }
                else
                {
                    gen.Emit(OpCodes.Ldloc, curLocals[varPointer]);
                    pointer++;
                    CreateExpression(gen, commands, ref pointer);
                    pointer++;
                    CreateExpression(gen, commands, ref pointer);
                }
            }

            return type;
        }

        static void LoadVarValue(ILGenerator gen, string[] commands, ref int pointer)
        {
            pointer++;
            int dim = Convert.ToInt32(commands[pointer++].ToString());
            int varPointer = Convert.ToInt32(commands[pointer++].ToString());
            string strType = commands[pointer++];
            int curVarPointer = varPointer - methodParamCount[methodParamCount.Count - 1];
            if (dim == 0)
            {
                if (curVarPointer >= 0)
                {
                    gen.Emit(OpCodes.Ldloc, curLocals[curVarPointer]);
                }
                else
                {
                    gen.Emit(OpCodes.Ldarg, varPointer);
                }
            }
            else
            {
                if (dim == 1)
                {
                    gen.Emit(OpCodes.Ldloc, curLocals[curVarPointer]);
                    pointer++;
                    CreateExpression(gen, commands, ref pointer);

                    if (strType == "map")
                    {
                        gen.Emit(OpCodes.Callvirt, curTypes[curVarPointer].GetMethod("get_Item", 
                                                    new Type[] { curTypes[curVarPointer].GetGenericArguments()[0] }));
                    }
                    else
                    {
                        gen.Emit(OpCodes.Ldelem, GetTypeByString(strType));
                    }
                }
                else
                {
                    gen.Emit(OpCodes.Ldloc, curLocals[varPointer]);
                    pointer++;
                    CreateExpression(gen, commands, ref pointer);
                    pointer++;
                    CreateExpression(gen, commands, ref pointer);
                    gen.Emit(OpCodes.Call, GetTypeByString(strType, true, true).GetMethod("Get", new Type[] {typeof(int), typeof(int)}));
                }
            }
        }

        static void CallFunction(ILGenerator gen, string[] commands, ref int startPointer)
        {
            int funcIndex = Convert.ToInt32(commands[++startPointer]);
            int funcArgsCount = Convert.ToInt32(commands[++startPointer]);
            startPointer++;
            for (int i = 0; i < funcArgsCount; ++i )
            {
                CreateExpression(gen, commands, ref startPointer);
            }

            gen.Emit(OpCodes.Call, methods[funcIndex]);
        }

        static void CreateExpression(ILGenerator gen, string[] commands, ref int startPointer)
        {
            string type = commands[++startPointer];
            startPointer++;
            
            int commandsSize = Convert.ToInt32(commands[startPointer++].ToString());
            int pointer = startPointer;
            for (int i = 0; i < commandsSize; ++i )
            {
                if (commands[pointer] == "op")
                {
                    gen.Emit(GetOpCodeForOperator(commands[pointer + 1]));
                    pointer += 2;
                }
                else
                {
                    if (IsType(commands[pointer]))
                    {
                        if (commands[pointer] == "CONST_INTEGER")
                        {
                            gen.Emit(OpCodes.Ldc_I4, Convert.ToInt32(commands[pointer + 1].ToString()));
                            if (type == "float")
                            {
                                gen.Emit(OpCodes.Conv_R4);
                            }
                            pointer += 2;
                        }
                        else
                        {
                            if (commands[pointer] == "CONST_FLOAT")
                            {
                                string num = commands[pointer + 1].ToString();
                                gen.Emit(OpCodes.Ldc_R4, float.Parse(num, CultureInfo.InvariantCulture));
                                pointer += 2;
                            }
                            else
                            {
                                if (commands[pointer] == "CONST_STRING")
                                {
                                    gen.Emit(OpCodes.Ldstr, commands[pointer + 1].Substring(1, commands[pointer + 1].Length - 2));
                                    pointer += 2;
                                }
                                else
                                {
                                    gen.Emit(OpCodes.Ldc_I4, StringToChar(commands[pointer + 1]));
                                    pointer += 2;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (commands[pointer] == "variable")
                        {
                            LoadVarValue(gen, commands, ref pointer);
                        }
                        else
                        {
                            CallFunction(gen, commands, ref pointer);
                        }
                    }
                }
            }

            startPointer = pointer;
        }

        static void CreateCondition(ILGenerator gen, string[] commands, ref int startPointer)
        {
            int commandsSize = Convert.ToInt32(commands[++startPointer].ToString());
            int pointer = ++startPointer;
            for (int i = 0; i < commandsSize; ++i)
            {
                if (commands[pointer] == "expression")
                {
                    CreateExpression(gen, commands, ref pointer);
                }
                else
                {
                    pointer++;
                    gen.Emit(GetOpCodeForOperator(commands[pointer++]));
                }
            }

            startPointer = pointer;
        }

        static void DeclareVar(ILGenerator gen, string[] args, int startPointer)
        {
            if (args[startPointer + 2] == "0")
            {
                curLocals.Add(gen.DeclareLocal(GetTypeByString(args[startPointer + 1])));
                curTypes.Add(GetTypeByString(args[startPointer + 1]));
                curScopes.Add(curScope);
            }
            else
            {
                if (args[startPointer + 2] == "1")
                {
                    var arr = gen.DeclareLocal(GetTypeByString(args[startPointer + 1], true));
                    int pointer = 4;
                    CreateExpression(gen, args, ref pointer);
                    gen.Emit(OpCodes.Newarr, GetTypeByString(args[startPointer + 1]));
                    gen.Emit(OpCodes.Stloc, arr);
                    curLocals.Add(arr);
                    curTypes.Add(GetTypeByString(args[startPointer + 1], true));
                    curScopes.Add(curScope);
                }
                else
                {
                    var arrType = GetTypeByString(args[startPointer + 1], true, true);
                    var arr = gen.DeclareLocal(arrType);
                    int pointer = 4;
                    CreateExpression(gen, args, ref pointer);
                    pointer++;
                    CreateExpression(gen, args, ref pointer);
                    gen.Emit(OpCodes.Newobj, arrType.GetConstructor(new Type[] {typeof(int), typeof(int)}));
                    gen.Emit(OpCodes.Stloc, arr);
                    curLocals.Add(arr);
                    curTypes.Add(arrType);
                    curScopes.Add(curScope);
                }
            }
        }

        static void DeclareMap(ILGenerator gen, string[] args)
        {
            var keyType = GetTypeByString(args[1]);
            var valType = GetTypeByString(args[2]);

            Type type = typeof(SortedDictionary<,>).MakeGenericType(new Type[] {keyType, valType });
            gen.Emit(OpCodes.Newobj, type.GetConstructor(new Type[] {}));
            var arr = gen.DeclareLocal(type);
            gen.Emit(OpCodes.Stloc, arr);
            curLocals.Add(arr);
            curTypes.Add(type);
            curScopes.Add(curScope);
        }

        static void RemoveCurScopes()
        {
            for (int i = curScopes.Count - 1; i >= 0; --i)
            {
                if (curScopes[i] == curScope)
                {
                    curScopes.RemoveAt(i);
                    curLocals.RemoveAt(i);
                    curTypes.RemoveAt(i);
                }
                else
                {
                    break;
                }
            }
        }

        static void Main(string[] args)
        {
            string[] lines = File.ReadAllLines("D:\\Code\\3_1\\git\\SOMETHINGUDONTWANNASEE\\Debug\\cmdLog.txt");

            AssemblyBuilder build = AppDomain.CurrentDomain.DefineDynamicAssembly(
                new System.Reflection.AssemblyName("CustomCompiler program"),
                AssemblyBuilderAccess.RunAndSave);
            ModuleBuilder module = build.DefineDynamicModule(args[0], args[0] + ".exe");
            TypeBuilder prog = module.DefineType("Program", TypeAttributes.Class | TypeAttributes.Public);
            builder = null;

            Stack<Label> ifLabels = new Stack<Label>();
            Stack<Label> ifElseLabels = new Stack<Label>();
            Stack<Label> whileLabels = new Stack<Label>();
            Stack<Label> whileLabelsExit = new Stack<Label>();
            Stack<Label> forExitLabels = new Stack<Label>();
            Stack<Label> forLabels = new Stack<Label>();

            foreach (string line in lines)
            {
                string[] tokens = Split(line);
                if (tokens[0] == "func")
                {
                    string funcName = tokens[1];
                    var retType = GetTypeByString(tokens[2]);
                    int count = Convert.ToInt32(tokens[3].ToString());
                    Type[] types = new Type[count];
                    for (int i = 0; i < count; ++i)
                    {
                        types[i] = GetTypeByString(tokens[4 + i]);
                    }

                    builder = CreateNewMethod(prog, funcName, retType, types);
                    methods.Add(builder);
                    methodParamCount.Add(count);

                    if (funcName == "main")
                    {
                        build.SetEntryPoint(builder, PEFileKinds.ConsoleApplication);
                    }
                }

                if (tokens[0] == "func_end")
                {
                    builder.GetILGenerator().Emit(OpCodes.Ret);
                    curLocals.Clear();
                    curTypes.Clear();
                    curScope = 0;
                    curScopes.Clear();
                }

                if (tokens[0] == "new_var")
                {
                    DeclareVar(builder.GetILGenerator(), tokens, 0);
                }

                if (tokens[0] == "new_map")
                {
                    DeclareMap(builder.GetILGenerator(), tokens);
                }

                if (tokens[0] == "assign")
                {
                    int pointer = 2;
                    if (tokens[1] == "0")
                    {
                        CreateExpression(builder.GetILGenerator(), tokens, ref pointer);
                        LoadVariable(builder.GetILGenerator(), tokens, ref pointer);
                    }
                    else
                    {
                        var type = LoadVariable(builder.GetILGenerator(), tokens, ref pointer);
                        CreateExpression(builder.GetILGenerator(), tokens, ref pointer);
                        if (tokens[1] == "1")
                        {
                            builder.GetILGenerator().Emit(OpCodes.Stelem, type);
                        }
                        else
                        {
                            if (tokens[1] == "2")
                            {
                                string arrType = tokens[5];
                                builder.GetILGenerator().Emit(OpCodes.Call, GetTypeByString(arrType, true, true).GetMethod("Set",
                                                            new Type[] { typeof(int), typeof(int), GetTypeByString(arrType) }));
                            }
                            else
                            {
                                int p = Convert.ToInt32(tokens[4]) - methodParamCount[methodParamCount.Count - 1];
                                builder.GetILGenerator().Emit(OpCodes.Callvirt, curTypes[p].GetMethod("set_Item",
                                                new Type[] { curTypes[p].GetGenericArguments()[0], curTypes[p].GetGenericArguments()[1] }));
                            }
                        }
                    }
                }

                if (tokens[0] == "output")
                {
                    int pointer = 1;
                    CreateExpression(builder.GetILGenerator(), tokens, ref pointer);
                    string type = tokens[2];
                    Type curType = GetTypeByString(type);
                    builder.GetILGenerator().Emit(OpCodes.Call,
                                 typeof(Console).GetMethod("Write",
                                 new Type[] { curType }));
                }

                if (tokens[0] == "input")
                {
                    int pointer = 2;
                    if (tokens[1] == "0")
                    {
                        builder.GetILGenerator().EmitCall(OpCodes.Call,
                               typeof(Console).GetMethod("ReadLine"), new[] { typeof(string) });
                        if (tokens[5] == "float")
                        {
                            builder.GetILGenerator().Emit(OpCodes.Call,
                                typeof(CultureInfo).GetMethod("get_InvariantCulture"));
                            builder.GetILGenerator().Emit(OpCodes.Call,
                                typeof(Convert).GetMethod("ToSingle", new[] { typeof(string), typeof(IFormatProvider) }));
                        }
                        else
                        {
                            builder.GetILGenerator().Emit(OpCodes.Call,
                                typeof(Convert).GetMethod(GetConvertMethodName(GetTypeByString(tokens[5])), new[] { typeof(string) }));
                        }

                        LoadVariable(builder.GetILGenerator(), tokens, ref pointer);
                    }
                    else
                    {
                        var type = LoadVariable(builder.GetILGenerator(), tokens, ref pointer);
                        builder.GetILGenerator().EmitCall(OpCodes.Call,
                                typeof(Console).GetMethod("ReadLine"), new[] { typeof(string) });
                        if (type == typeof(float))
                        {
                            builder.GetILGenerator().Emit(OpCodes.Call,
                                typeof(CultureInfo).GetMethod("get_InvariantCulture"));
                            builder.GetILGenerator().Emit(OpCodes.Call,
                                typeof(Convert).GetMethod("ToSingle", new[] { typeof(string), typeof(IFormatProvider) }));
                        }
                        else
                        {
                            builder.GetILGenerator().Emit(OpCodes.Call,
                                    typeof(Convert).GetMethod(GetConvertMethodName(type), new[] { typeof(string) }));
                        }

                        if (tokens[1] == "1")
                        {
                            builder.GetILGenerator().Emit(OpCodes.Stelem, type);
                        }
                        else
                        {
                            string arrType = tokens[5];
                            builder.GetILGenerator().Emit(OpCodes.Call, GetTypeByString(arrType, true, true).GetMethod("Set",
                                                        new Type[] { typeof(int), typeof(int), GetTypeByString(arrType) }));
                        }
                    }
                }

                if (tokens[0] == "return")
                {
                    if (tokens[1] == "empty")
                    {
                        builder.GetILGenerator().Emit(OpCodes.Ret);
                    }
                    else
                    {
                        var needType = GetTypeByString(tokens[1]);
                        var gotType = GetTypeByString(tokens[2]);
                        int pointer = 3;
                        CreateExpression(builder.GetILGenerator(), tokens, ref pointer);
                        if (needType != gotType && needType == typeof(float))
                        {
                            builder.GetILGenerator().Emit(OpCodes.Conv_R4);
                        }
                        builder.GetILGenerator().Emit(OpCodes.Ret);
                    }
                }

                if (tokens[0] == "if")
                {
                    int pointer = 1;
                    CreateCondition(builder.GetILGenerator(), tokens, ref pointer);
                    Label label = builder.GetILGenerator().DefineLabel();
                    ifLabels.Push(label);
                    builder.GetILGenerator().Emit(OpCodes.Brfalse, label);
                    curScope++;
                }

                if (tokens[0] == "end_if")
                {
                    if (tokens[1] == "else")
                    {
                        Label elseLabel = builder.GetILGenerator().DefineLabel();
                        ifElseLabels.Push(elseLabel);
                        builder.GetILGenerator().Emit(OpCodes.Br, elseLabel);
                    }
                    Label label = ifLabels.Pop();
                    builder.GetILGenerator().MarkLabel(label);
                    RemoveCurScopes();
                    curScope--;
                }

                if (tokens[0] == "else")
                {
                    curScope++;
                }

                if (tokens[0] == "end_else")
                {
                    Label label = ifElseLabels.Pop();
                    builder.GetILGenerator().MarkLabel(label);
                    RemoveCurScopes();
                    curScope--;
                }

                if (tokens[0] == "for")
                {
                    curScope++;
                    int pointer = 1;
                    DeclareVar(builder.GetILGenerator(), tokens, pointer);

                    pointer = 6;
                    CreateExpression(builder.GetILGenerator(), tokens, ref pointer);
                    LoadVariable(builder.GetILGenerator(), tokens, ref pointer);

                    Label label = builder.GetILGenerator().DefineLabel();
                    builder.GetILGenerator().MarkLabel(label);
                    forLabels.Push(label);
                    
                    Label exit = builder.GetILGenerator().DefineLabel();
                    forExitLabels.Push(exit);

                    CreateCondition(builder.GetILGenerator(), tokens, ref pointer);
                    builder.GetILGenerator().Emit(OpCodes.Brfalse, exit);

                    curScope++;
                }

                if (tokens[0] == "end_for")
                {
                    int pointer = 3;
                    CreateExpression(builder.GetILGenerator(), tokens, ref pointer);
                    LoadVariable(builder.GetILGenerator(), tokens, ref pointer);

                    builder.GetILGenerator().Emit(OpCodes.Br, forLabels.Pop());
                    Label label = forExitLabels.Pop();
                    builder.GetILGenerator().MarkLabel(label);

                    RemoveCurScopes();
                    curScope--;

                    RemoveCurScopes();
                    curScope--;
                }

                if (tokens[0] == "while")
                {
                    int pointer = 1;

                    Label label = builder.GetILGenerator().DefineLabel();
                    builder.GetILGenerator().MarkLabel(label);
                    whileLabels.Push(label);
                    Label exit = builder.GetILGenerator().DefineLabel();
                    whileLabelsExit.Push(exit);

                    CreateCondition(builder.GetILGenerator(), tokens, ref pointer);
                    builder.GetILGenerator().Emit(OpCodes.Brfalse, exit);
                    curScope++;
                }

                if (tokens[0] == "end_while")
                {
                    builder.GetILGenerator().Emit(OpCodes.Br, whileLabels.Pop());
                    Label label = whileLabelsExit.Pop();
                    builder.GetILGenerator().MarkLabel(label);
                    RemoveCurScopes();
                    curScope--;
                }
            }
            prog.CreateType();

            build.Save(args[0] + ".exe");
        }
    }
}
