namespace C__website.Models{

    public class Orientation{

        public AspectRatio AR { get; set; }

    }

    public enum AspectRatio
    {
        Default,
        Landscape,
        Portrait,
        Square,
        Fill
    }

}