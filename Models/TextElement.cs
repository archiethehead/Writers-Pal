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

    public static class JsonHandler {

        public static TextElement JsonToTextElement(JsonElement json) {

            UInt16 initID = json.GetProperty("ID").GetUInt16()!;
            string jsonType = json.GetProperty("Type").GetString()!;
            TextElementType initType = Enum.Parse<TextElementType>(jsonType);

            TextElement newElement = new TextElement(initType, initID);
            newElement.text = json.GetProperty("text").GetString()!;

            return newElement;
        
        }

        public static string TextElementToJson(TextElement element) {

            return JsonSerializer.Serialize(new
            {
                id = element.ID,
                type = element.type.ToString(),
                text = element.text
            });
        
        }

    }

    public class TextElement {

        public UInt16 ID { get; init; } = 1;
        public TextElementType type { get; set; }
        public string text { get; set; } = "";

        public TextElement(TextElementType initType, UInt16 initID) {

            type = initType;
            ID = initID;

        }

    }

}
