using Avalonia.Data;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace Writers_Pal.Models {

    public enum TextElementType { 
    
        Description,
        Name,
        Dialogue,
        Setting,
        Stage
    
    }

    static class JsonHandler {

        static TextElement JsonToTextElement(JsonElement json) {

            Int32 initID = json.GetProperty("ID").GetInt32()!;
            string jsonType = json.GetProperty("Type").GetString()!;
            TextElementType initType = Enum.Parse<TextElementType>(jsonType);

            TextElement newElement = new TextElement(initType, initID);
            newElement.text = json.GetProperty("text").GetString()!;

            return newElement;
        
        }
    
    }

    public class TextElement {

        public Int32 ID { get; init; } = 1;
        public TextElementType type { get; set; }
        public string text { get; set; } = "";

        public TextElement(TextElementType initType, Int32 initID) {

            type = initType;
            ID = initID;

        }

    }

}
