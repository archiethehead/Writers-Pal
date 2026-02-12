using Avalonia.Data;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Writers_Pal.Models
{
    
    internal class TextElement {

        public int ID;
        public string? type;
        public string? text;

    }

    internal class SceneHeading : TextElement {

        public string? time;
        public string? location;

    }

    internal class Dialogue : TextElement {

        public string? character;
    
    }

    internal class Description : TextElement {}

    internal class Stage : TextElement {}

}
