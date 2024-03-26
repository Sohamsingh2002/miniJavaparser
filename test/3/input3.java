public class input3 {
    public static void main(String[] args) {
        Scanner myObj = new Scanner();
        String s="";
        String t="abcd";
        int p;
        for(p=5;p>=0;p--){
            if(p!=0){
                s=s+t;
            }
            if(p==0){
                s=s+s;
            }
        }
    }
}
