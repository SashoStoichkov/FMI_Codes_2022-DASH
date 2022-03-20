using System;

namespace Test
{
    class Program
    {
        static Tuple<int, int> getBestRectSlow(int lCubes, int rCubes, double ratio)
        {
            double closestRatio = -rCubes;
            Tuple<int, int> ans = Tuple.Create<int, int>(0, 0);
            
            for (int w = 1; w <= rCubes; w++)
            {
                for (int h = 1; h <= rCubes; h++)
                {
                    if (lCubes <= w * h && w * h <= rCubes)
                    {
                        double curr = (double)w / (double)h;
                        if (Math.Abs(curr - ratio) < Math.Abs(closestRatio - ratio))
                        {
                            ans = Tuple.Create<int, int>(w, h);
                            closestRatio = curr;
                        }
                    }
                }
            }

            return ans;
        }

        static Tuple<int, int> GetBestRect(int lCubes, int rCubes, double ratio)
        {
            double closestRatio = -1;
            Tuple<int, int> ans = Tuple.Create<int, int>(-1, -1);

            void update(int w, int h)
            {
                double curr = (double)w / (double)h;
                if(lCubes<=w*h && w*h<=rCubes && (Math.Abs(ratio-curr)<Math.Abs(ratio-closestRatio) || ans.Item1==-1))
                {
                    ans = Tuple.Create<int, int>(w, h);
                    closestRatio = curr;
                }
            }

            for (int w = 1; w <= rCubes; w++)
            {
                int minH = (lCubes+w-1) / w;
                int maxH = rCubes / w;

                int mid1 = (int)Math.Floor(w / ratio);
                int mid2 = mid1 + 1;

                update(w, minH);
                update(w, maxH);
                update(w, mid1);
                update(w, mid2);
            }

            return ans;
        }

        static void Main(string[] args)
        {
            //Console.WriteLine($"{getBestRectSlow(90, 100, 1.5)} vs {GetBestRect(90, 100, 1.5)}");
            //Console.WriteLine($"{getBestRectSlow(90, 100, 1.78)} vs {GetBestRect(90, 100, 1.78)}");
            //Console.WriteLine($"{getBestRectSlow(90, 100, 1)} vs {GetBestRect(90, 100, 1)}");
            //Console.WriteLine($"{getBestRectSlow(995, 1000, 1)} vs {GetBestRect(995, 1000, 1)}");
            //Console.WriteLine($"{getBestRectSlow(995, 1000, 1)} vs {GetBestRect(995, 1000, 1)}");
            Console.WriteLine(GetBestRect(300, 400, 1.86));
        }
    }
}
