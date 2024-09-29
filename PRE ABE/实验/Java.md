# Java



[Java程序设计]([翁恺 Java程序设计_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1DJ411B7cG/?spm_id_from=333.337.search-card.all.click&vd_source=658588b117b821559d189ce730ded61c))



```java
public class Test001{
	public static void main(String arg[]){
		System.out.println("hello,world");
	}
}
```



```java
import java.util.Scanner;

public class Main
{
	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
    	System.out.println("Enter your name: ");
    	String inp = input.next();
    	System.out.println("Hello, " + inp);
	}
}
```



从软件工程的角度来说，一行最好只定义一个变量。