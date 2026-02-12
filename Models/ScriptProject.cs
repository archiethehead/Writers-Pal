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
    internal class ScriptProject
    {

        public Repository? currentRepo;

        public int RandomID() {

            Random rng = new Random();
            int newID;

            do {

                newID = rng.Next(0, 65535);

            } while (IsIDTaken(newID));

            return newID;
        
        }

        public bool IsIDTaken(int ID)
        {

            return false;

        }

    }

}
