import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;
import java.util.StringTokenizer;
public class cjam{
        static class Reader
    {
        final private int BUFFER_SIZE = 1 << 16;
        private DataInputStream din;
        private byte[] buffer;
        private int bufferPointer, bytesRead;
 
        public Reader()
        {
            din = new DataInputStream(System.in);
            buffer = new byte[BUFFER_SIZE];
            bufferPointer = bytesRead = 0;
        }
 
        public Reader(String file_name) throws IOException
        {
            din = new DataInputStream(new FileInputStream(file_name));
            buffer = new byte[BUFFER_SIZE];
            bufferPointer = bytesRead = 0;
        }
 
        public String readLine() throws IOException
        {
            byte[] buf = new byte[64]; // line length
            int cnt = 0, c;
            while ((c = read()) != -1)
            {
                if (c == '\n')
                    break;
                buf[cnt++] = (byte) c;
            }
            return new String(buf, 0, cnt);
        }
        /*
            Added by me.
        */
        public String readWord() throws IOException
        {
            byte[] buf = new byte[64]; // line length
            int cnt = 0, c;
            while ((c = read()) != -1)
            {
                if (c == ' ')
                    break;
                buf[cnt++] = (byte) c;
            }
            return new String(buf, 0, cnt);
        }
        public int nextInt() throws IOException
        {
            int ret = 0;
            byte c = read();
            while (c <= ' ')
                c = read();
            boolean neg = (c == '-');
            if (neg)
                c = read();
            do
            {
                ret = ret * 10 + c - '0';
            }  while ((c = read()) >= '0' && c <= '9');
 
            if (neg)
                return -ret;
            return ret;
        }
 
        public long nextLong() throws IOException
        {
            long ret = 0;
            byte c = read();
            while (c <= ' ')
                c = read();
            boolean neg = (c == '-');
            if (neg)
                c = read();
            do {
                ret = ret * 10 + c - '0';
            }
            while ((c = read()) >= '0' && c <= '9');
            if (neg)
                return -ret;
            return ret;
        }
 
        public double nextDouble() throws IOException
        {
            double ret = 0, div = 1;
            byte c = read();
            while (c <= ' ')
                c = read();
            boolean neg = (c == '-');
            if (neg)
                c = read();
 
            do {
                ret = ret * 10 + c - '0';
            }
            while ((c = read()) >= '0' && c <= '9');
 
            if (c == '.')
            {
                while ((c = read()) >= '0' && c <= '9')
                {
                    ret += (c - '0') / (div *= 10);
                }
            }
 
            if (neg)
                return -ret;
            return ret;
        }
 
        private void fillBuffer() throws IOException
        {
            bytesRead = din.read(buffer, bufferPointer = 0, BUFFER_SIZE);
            if (bytesRead == -1)
                buffer[0] = -1;
        }
 
        private byte read() throws IOException
        {
            if (bufferPointer == bytesRead)
                fillBuffer();
            return buffer[bufferPointer++];
        }
 
        public void close() throws IOException
        {
            if (din == null)
                return;
            din.close();
        }
    } //static class Reader
 
    public static void main(String... args) throws IOException{

        final boolean DEBUG = false;

        Reader s = new Reader(); //Initialize a reader!
        int t = s.nextInt();
        for(int t_count=1; t_count<t+1; t_count++){
         long num;
         num = s.nextLong();
        int loop_counter=0;
        while(true){
            if(DEBUG) System.out.println("Loop Iteration: " + loop_counter);
            long dummy = num;
            int swaps = 0, d_count = 0, d_equals = 0;
            while(true){
                boolean fl = false;
                if(DEBUG) System.out.printf("\tNum :%ld, Dummy: %ld\n", num, dummy);
                if(dummy <= 0) break;
                long dummy2 = dummy;
                while(dummy2 > 9){
                    if(dummy2%10 < (dummy2/10) % 10) {fl=true; break;}
                    dummy2/=10;
                } //if(dummy%10 < (dummy/10) % 10)
                if(fl) {
                    //num+=9*pow(10, d_count);
                    //num-=(dummy%10);
                    do {
                        //if(DEBUG) printf("      NUM==== %lld\n", num);
                        num = num - (long) Math.pow(10, d_count-d_equals)*(((long)(num / (long)(Math.pow(10, d_count-d_equals))))%10);
                        if(DEBUG) System.out.printf(" equals: %d: , count: %d  --num%%10 power dC- dE: %lld-- \n", d_equals,d_count,num);
                        num = num + (long)9*(long)Math.pow(10, d_count-d_equals);
                        if(DEBUG) System.out.printf("    --+9*blah: %lld-- \n", num);
                        d_equals--; 
                    }while(d_equals > -1);
                    num-=(long)(Math.pow(10, d_count+1));
                    swaps++;
                    d_equals = 0;
                    if(DEBUG) System.out.printf("    >>>changed @pos %d<<<   \n", d_count);
                }
                else if(dummy%10 == (dummy/10) % 10){
                    d_equals++;
                }
                else d_equals = 0;
                d_count++;
                dummy=num/(long)Math.pow(10, d_count); //decrement.
                
            } //inner loop
            //if(DEBUG) printf("  \t\tSWAPS: %lld", swaps);
            loop_counter++;
            if(swaps == 0 || loop_counter > 19) break;
            
        } //outer loop
        //printf("Case #%d: %lld\n", t_count, num);
         System.out.println("Case #" + t_count + ": " + num);
        }

    } //main


} //public class Contest