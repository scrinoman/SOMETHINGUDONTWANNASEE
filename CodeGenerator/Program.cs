using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Reflection.Emit;
using System.Globalization;

namespace CodeGenerator
{
    class Program
    {
        static List<LocalBuilder> curLocals = new List<LocalBuilder>();

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
            //if (op == ">=") return;
            //if (op == "<=") return;
            if (op == "==") return OpCodes.Ceq;
            //if (op == "!=") { return OpCodes.Ceq; };
            if (op == "&&") return OpCodes.And;
            if (op == "||") return OpCodes.Or;
            //if (op == "!") return OpCodes.

            return OpCodes.Nop;
        }

        static Type GetTypeByString(string type, bool arrayType = false)
        {
            if (type == "int") return arrayType ? typeof(int[]) : typeof(int);
            if (type == "float") return arrayType ? typeof(float[]) : typeof(float);
            if (type == "string") return arrayType ? typeof(string[]) : typeof(string);
            if (type == "char") return arrayType ? typeof(char[]) :  typeof(char);
            
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
            Type type = GetTypeByString(commands[pointer++].ToString());

            if (dim == 0)
            {
                gen.Emit(OpCodes.Stloc, curLocals[varPointer]);
            }
            else
            {
                if (dim == 1)
                {
                    gen.Emit(OpCodes.Ldloc, curLocals[varPointer]);
                    pointer++;
                    CreateExpression(gen, commands, ref pointer);
                }
                else //TODO
                {
                }
            }

            return type;
        }

        static void LoadVarValue(ILGenerator gen, string[] commands, ref int pointer)
        {
            pointer++;
            int dim = Convert.ToInt32(commands[pointer++].ToString());
            int varPointer = Convert.ToInt32(commands[pointer++].ToString());
            Type type = GetTypeByString(commands[pointer++].ToString());
            if (dim == 0)
            {
                gen.Emit(OpCodes.Ldloc, curLocals[varPointer]);
            }
            else
            {
                if (dim == 1)
                {
                    gen.Emit(OpCodes.Ldloc, curLocals[varPointer]);
                    pointer++;
                    CreateExpression(gen, commands, ref pointer);

                    gen.Emit(OpCodes.Ldelem, type);
                }
                else //== todo
                {
                    gen.Emit(OpCodes.Ldloc, curLocals[varPointer]);
                    pointer++;
                    CreateExpression(gen, commands, ref pointer);
                    pointer++;

                    gen.Emit(OpCodes.Ldelem, type);
                }
            }
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
                                    gen.Emit(OpCodes.Ldc_I4, Convert.ToChar(commands[pointer + 1].Substring(1, 1)));
                                    pointer += 2;
                                }
                            }
                        }
                    }
                    else
                    {
                        LoadVarValue(gen, commands, ref pointer);
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

        static void DeclareVar(ILGenerator gen, string[] args)
        {
            if (args[2] == "0")
            {
                curLocals.Add(gen.DeclareLocal(GetTypeByString(args[1])));
            }
            else
            {
                if (args[2] == "1")
                {
                    var arr = gen.DeclareLocal(GetTypeByString(args[1], true));
                    int pointer = 4;
                    CreateExpression(gen, args, ref pointer);
                    gen.Emit(OpCodes.Newarr, GetTypeByString(args[1]));
                    gen.Emit(OpCodes.Stloc, arr);
                    curLocals.Add(arr);
                }
                else
                {
                    var arr = gen.DeclareLocal(GetTypeByString(args[1], true));
                    int pointer = 4;
                    CreateExpression(gen, args, ref pointer);
                    int i = 0;
                    for (i = 0; i < args.Length; ++i )
                    {
                        if (args[i] == "sd")
                        {
                            break;
                        }
                    }
                    pointer++;
                    CreateExpression(gen, args, ref pointer);
                    gen.Emit(OpCodes.Mul);
                    gen.Emit(OpCodes.Newarr, GetTypeByString(args[1]));
                    gen.Emit(OpCodes.Stloc, arr);
                    curLocals.Add(arr);
                }
            }
        }

        static void Main(string[] args)
        {
            string[] lines = System.IO.File.ReadAllLines("cmdLog.txt");

            AssemblyBuilder build = AppDomain.CurrentDomain.DefineDynamicAssembly(
                new System.Reflection.AssemblyName("CustomCompiler program"), 
                AssemblyBuilderAccess.RunAndSave);
            ModuleBuilder module = build.DefineDynamicModule(args[0], args[0] + ".exe");
            TypeBuilder prog = module.DefineType("Program", TypeAttributes.Class | TypeAttributes.Public);
            MethodBuilder builder = prog.DefineMethod("_STRANGE_USELESS_METHOD_ONLY_FOR_THIS_LINE_", MethodAttributes.HideBySig);
            builder.GetILGenerator().Emit(OpCodes.Ret);
            curLocals.Clear();
            Stack<Label> ifLabels = new Stack<Label>();
            Stack<Label> whileLabels = new Stack<Label>();
            Stack<Label> whileLabelsExit = new Stack<Label>();

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
                    
                    if (funcName == "main")
                    {
                        build.SetEntryPoint(builder, PEFileKinds.ConsoleApplication);
                    }
                }
                else
                {
                    if (tokens[0] == "func_end")
                    {
                        builder.GetILGenerator().Emit(OpCodes.Ret);
                        curLocals.Clear();
                    }

                    if (tokens[0] == "new_var")
                    {
                        DeclareVar(builder.GetILGenerator(), tokens);
                    }

                    if (tokens[0] == "assign")
                    {
                        int pointer = 2;
                        if (tokens[1] == "0")
                        {
                            var type = LoadVariable(builder.GetILGenerator(), tokens, ref pointer);
                            CreateExpression(builder.GetILGenerator(), tokens, ref pointer);
                            builder.GetILGenerator().Emit(OpCodes.Stelem, type);
                        }
                        else
                        {
                            CreateExpression(builder.GetILGenerator(), tokens, ref pointer);
                            LoadVariable(builder.GetILGenerator(), tokens, ref pointer);
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
                            builder.GetILGenerator().Emit(OpCodes.Stelem, type);
                        }
                        else
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
                    }

                    if (tokens[0] == "end_if")
                    {
                        Label label = ifLabels.Pop();
                        builder.GetILGenerator().MarkLabel(label);
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
                    }

                    if (tokens[0] == "end_while")
                    {
                        builder.GetILGenerator().Emit(OpCodes.Br, whileLabels.Peek());
                        Label label = whileLabelsExit.Pop();
                        builder.GetILGenerator().MarkLabel(label);
                    }
                }
            }
            prog.CreateType();
            
            build.Save(args[0] + ".exe");
        }
    }
}
