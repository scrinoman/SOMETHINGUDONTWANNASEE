﻿using System;
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

            return OpCodes.Nop;
        }

        static Type GetTypeByString(string type)
        {
            if (type == "int") return typeof(int);
            if (type == "float") return typeof(float);
            if (type == "string") return typeof(string);
            if (type == "char") return typeof(char);
            
            return typeof(void);
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
        
        static void CreateExpression(ILGenerator gen, string[] commands)
        {
            string type = commands[1];
            
            int commandsSize = Convert.ToInt32(commands[2].ToString());
            int pointer = 3;
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
                                    gen.Emit(OpCodes.Ldc_I4, Convert.ToChar(commands[pointer + 1].ToString()[1]));
                                    pointer += 2;
                                }
                            }
                        }
                    }
                    else
                    {

                    }
                }
            }
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
                    var arr = gen.DeclareLocal(GetTypeByString(args[1]));
                    CreateExpression(gen, args.Skip(4).Take(args.Length - 4).ToArray());
                    gen.Emit(OpCodes.Newarr, GetTypeByString(args[1]));
                    gen.Emit(OpCodes.Stloc, arr);
                    curLocals.Add(arr);
                }
                else
                {
                    var arr = gen.DeclareLocal(GetTypeByString(args[1]));
                    CreateExpression(gen, args.Skip(4).Take(args.Length - 4).ToArray());
                    int i = 0;
                    for (i = 0; i < args.Length; ++i )
                    {
                        if (args[i] == "sd")
                        {
                            break;
                        }
                    }
                    CreateExpression(gen, args.Skip(i + 1).Take(args.Length - i - 1).ToArray());
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
            MethodBuilder builder = prog.DefineMethod("STRANGE_USELESS_METHOD_ONLY_FOR_THIS_LINE", MethodAttributes.HideBySig);
            builder.GetILGenerator().Emit(OpCodes.Ret);
            curLocals.Clear();

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

                    if (tokens[0] == "output")
                    {
                        CreateExpression(builder.GetILGenerator(), tokens.Skip(1).ToArray());
                        string type = tokens[2];
                        Type curType = GetTypeByString(type);
                        builder.GetILGenerator().Emit(OpCodes.Call,
                                     typeof(Console).GetMethod("WriteLine",
                                     new Type[] { curType }));
                    }
                }
            }
            prog.CreateType();
            
            build.Save(args[0] + ".exe");
        }
    }
}
