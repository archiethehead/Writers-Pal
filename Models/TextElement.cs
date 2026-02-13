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

            UInt16 initID = json.GetProperty("id").GetUInt16()!;
            string jsonType = json.GetProperty("type").GetString()!;
            TextElementType initType = Enum.Parse<TextElementType>(jsonType);

            TextElement newElement;
            newElement.ID = initID;
            newElement.type = initType;

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

    public struct TextElement {

        public UInt16 ID;
        public TextElementType type;
        public string text;

    };

}
