using LibGit2Sharp;
using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Intrinsics.Arm;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using static System.Random;

namespace Writers_Pal.Models {

    internal class ScriptProject {

        public string projectName { get; private set; } = "";
        public string projectAuthor { get; private set; } = "";
        public string authorEmail { get; private set; } = "";
        public List<TextElement> TextArray { get; private set; } = new List<TextElement>();
        public Repository? currentRepo { get; private set; }

        public ScriptProject(string name, string email, string filepath, bool creating) {

            projectAuthor = name;
            authorEmail = email;

            if (creating) { CreateProject(filepath); }

            OpenProject(filepath);
        
        }

        private void CreateProject(string filepath) {
            
            Directory.CreateDirectory(filepath);
            currentRepo = VersionControl.CreateNewRepo(filepath);

            //File.WriteAllText(Path.Combine(filepath, "project.json"), 
            //    JsonSerializer.Serialize(new { elements = Array.Empty<object>()}));
        
        }

        private void OpenProject(string filepath) {

            currentRepo = VersionControl.CreateNewRepo(filepath);

            string projectPath = Path.Combine(filepath, "project.json");

            JsonDocument projectJson = JsonDocument.Parse(File.ReadAllText(projectPath));
            TextArray.Clear();

            foreach (JsonElement index in projectJson.RootElement.GetProperty("elements").EnumerateArray()) {

                TextArray.Add(JsonHandler.JsonToTextElement(index));
            
            }   

        }

    }

    static public class IDGenerator {

        // Max Size of an unsigned, 16 bit int is 65535, going over this will result in countback
        // and ID corruption.

        private const UInt16 ID_SIZE = 20000;
        private static readonly bool[] assigned = new bool[ID_SIZE];
        private static readonly Random rng = new Random();



        static public UInt16 GenerateID() {

            UInt16 newID;

            do {

                newID = RandomID();

            } while (assigned[newID]);

            AssignID(newID);
            return newID;

        }

        public static void UnassignID(UInt16 ID) {

            assigned[ID] = false;
        
        }

        public static void AssignID(UInt16 ID)
        {

            assigned[ID] = true;

        }

        static public UInt16 RandomID() {

            return (ushort)rng.Next((ushort)0, ID_SIZE);

        }

    }

}
