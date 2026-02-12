using LibGit2Sharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Intrinsics.Arm;
using System.Text;
using System.Threading.Tasks;
using static System.Random;

namespace Writers_Pal.Models
{

    internal class ScriptProject {

        public string projectName { get; set; } = "";
        public string projectAuthor { get; set; } = "";
        public Repository? currentRepo { get; set; }

    }

    static public class IDGenerator {

        // Max Size of an unsigned, 16 bit int is 65535, going over this will result in countback
        // and ID corruption.

        public const UInt16 ID_SIZE = 10000;


        static public UInt16 AssignID()
            {

                UInt16 newID;

                do
                {

                    newID = RandomID();

                } while (IsIDTaken(newID));

                return newID;

            }

            static public UInt16 RandomID()
            {

                Random rng = new Random();
                return (ushort)rng.Next((ushort)0, ID_SIZE);

            }

            static public bool IsIDTaken(int ID)
            {

                return false;

            }

        }

    }
