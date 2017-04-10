#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char to_base_27(char input);
char *strrev(char *str);
int get_b27_index(char input);
int* back_to_27(long long input);

typedef struct{
  char value[193];
  int len;
} bigint;

bigint mult(bigint x, bigint y);
bigint new(char *input);
bigint minus(bigint x);
bigint to_base_10(int *input, int length);
bigint add(bigint x, bigint y);
bigint modulo(bigint x, bigint m);
bigint encrypt(bigint m, bigint e, bigint n);




void main(){
  FILE *fp;
  fp = fopen("INPUT.txt", "r");
  int ctr = 0;
  char twosev[] = "27";
  bigint tw = new(twosev);
  char pbuf[199], qbuf[199], ebuf[199];
  char buffer[199];
  int m[190], size;
  int *b27;
  bigint base10_message;
  bigint n, p, q, e, phin, minusp, minusq, c;
  char line[255];
  fgets(line, 255, fp);
  char sender[10], reciever[10], message[255];
  while (sscanf(line, "CASE %d", &ctr) == 1){
    printf("%s", line);
    fscanf(fp, "%[^,], %s\n", pbuf, qbuf);
    fscanf(fp, "%s\n", ebuf);
    printf("p = %s, q = %s, e = %s\n", pbuf,qbuf,ebuf);
    fgets(line, 255, fp);
    p = new(pbuf);
    q = new(qbuf);
    e = new(ebuf);
    n = mult(p,q);
    minusp = minus(p);
    minusq = minus(q);
    phin = mult(minusp, minusq);
    strcpy(buffer, n.value);
    strrev(buffer);
    printf("%s,%d\n", buffer,n.len);
    strcpy(buffer, phin.value);
    strrev(buffer);
    printf("%s,%d\n", buffer,phin.len);

    while(1){
      if(sscanf(line, "Alice will send the message: %[^to] to Bob", message) == 1){
        message[strlen(message) - 1] = '\0';
        printf("Alice sends %s!\n",message );
        for (int i=0; i<strlen(message);i++){
          //printf("b27: %d\n", get_b27_index(to_base_27(message[i])));
          m[i] = get_b27_index(to_base_27(message[i]));
        }
        base10_message = to_base_10(m, strlen(message));
        strcpy(buffer, base10_message.value);
        strrev(buffer);
        printf("Base 10: %s,%d\n", buffer,base10_message.len);
        c = encrypt(base10_message, e, n);
        // ===========================//
        //DO STUFF HERE               //
        //============================//

      // //  b27 = back_to_27(base10_message);
      //   size = b27[0]; //first element contains size
      //   printf("size: %d", size);
      //   for (int i=0; i<size; i++){
      //     printf("bb27: %d\n", b27[i]);
      //   }
        fgets(line,255,fp);
      }
      else if(sscanf(line, "Bob received the message: %[^from] from Alice", message) == 1){
        message[strlen(message) - 1] = '\0';
        printf("Bob recieves %s!\n",message );
        for (int i=0; i<strlen(message);i++){
          //printf("b27: %d\n", get_b27_index(to_base_27(message[i])));
          m[i] = get_b27_index(to_base_27(message[i]));
        }
        base10_message = to_base_10(m, strlen(message));
        strcpy(buffer, base10_message.value);
        strrev(buffer);
        printf("Base 10: %s,%d\n", buffer,base10_message.len);

        // ===========================//
        //DO STUFF HERE               //
        //============================//

      // //  b27 = back_to_27(base10_message);
      //   size = b27[0]; //first element contains size
      //   printf("size: %d", size);
      //   for (int i=1; i<size; i++){
      //     printf("bb27: %d\n", b27[i]);
      //   }

        fgets(line,255,fp);
      }
      else{
        break;
      }
    }
  // strcpy(buffer, y.value);
  // strrev(buffer);
  // printf("%s, value", buffer);     FOR PRINTING
  }
}
bigint new(char *input){
  bigint x;
  x.len = strlen(input);
  strcpy(x.value, strrev(input));
  return x;
}

bigint encrypt(bigint m, bigint e, bigint n){
  char *one = "1";
  bigint t = new(one);
  bigint c, temp = e;
  while(temp.value != "0"){
    printf("%s", temp.value);
    temp = minus(temp);
    t = mult(modulo(t,n),modulo(m,n));
    printf("%s\n", t.value);
  }
  t = modulo(t,n);
  return t;
}

bigint modulo(bigint x, bigint m){
  char temp[199], val[199];
  long long remainder;
  strcpy(val,m.value);
  strrev(val);
  strcpy(temp, x.value);
  strrev(temp);
  long long mod = atoi(val);
  printf("mod is %lld\t", mod);
  for(int i = 0; i<x.len; i++){
     remainder = (remainder * 10 + temp[i] - '0') % mod;
  }
  snprintf(temp, 199,"%lld", remainder);
  bigint y = new(temp);
  strrev(temp);
  printf("Modulo: %s\n", temp);
  return y;
}

bigint add(bigint x, bigint y){
  int xtemp[199] = { 0 }, ytemp[199] = { 0 };
  int xlen = x.len, ylen = y.len, carry = 0, maxlen;
  int result[199] = { 0 }, reslen;
  char buf[199];

  for(int i = 0; i<xlen; i++){
    xtemp[i] = x.value[i] - '0';
  }
  for(int i = 0; i<ylen; i++){
    ytemp[i] = y.value[i] - '0';
  }

  maxlen = (xlen > ylen) ? xlen : ylen;

  for (int i=0; i<maxlen; i++){
    result[i] = xtemp[i] + ytemp[i] + carry;
    carry = result[i] /10;
    result[i] %=10;
  }
  for(int i = maxlen; i>=0; i--){
    if (result[i] > 0){
      reslen = i+1;
      break;
    }
  }
  for(int i = 0; i<reslen; i++)
    buf[i] = result[i] + '0';
  buf[reslen] = '\0';
  x.len = reslen;
  strcpy(x.value, buf);
  strrev(buf);
  return x;

}

bigint mult(bigint x, bigint y){
  int max_length, xdigits[193],ydigits[193], prod[193] = { 0 }, temp;
  char ans[193], xtemp[193], ytemp[193];
  bigint z;
  strcpy(xtemp,x.value);
  strcpy(ytemp,y.value);

  for (int i=0; i<x.len ;i++){
    xdigits[i] = xtemp[i] - '0';
  //  printf("%d", xdigits[i]);
  }
  for (int i=0; i<y.len; i++){
    ydigits[i] = ytemp[i] - '0';
  //  printf("%d", ydigits[i]);
  }
  for (int i=0; i<y.len; i++){
    for (int j=0; j<x.len; j++){
      prod[i+j] +=  xdigits[j]*ydigits[i];
    }
  }
  // for (int i=0; i<x.len + y.len; i++){
  //   printf("%d,", prod[i]);
  // }

  for (int i=0; i< x.len + y.len; i++){
    temp = prod[i]/10;
    prod[i] %=10;
    prod[i+1] += temp;
  }


  int prodlen = 0;
  for (int i=x.len + y.len; i>=0; i--){
    if (prod[i] > 0){
      prodlen = i+1;
      break;
    }
  }
  for (int i = 0; i<prodlen; i++){
    ans[i] = prod[i] + '0';
  }
  ans[prodlen] = '\0';

  z.len = prodlen;
  strcpy(z.value, ans);
  return z;
}

int* back_to_27(long long input){
  int t[190];
  int j;
  int *result = malloc(190 * sizeof(*result));
  long long temp = input;
  int c = 0;
  while(temp !=0 ){
    t[c] = temp%27;
    temp /=27;
    c++; //hehe
  }

  for (int i = c-1, j = 1; i>=0; i--, j++){
    result[j] = t[i];
    printf("cbb: %d\n", result[j]);
  }
  result[0] = j; //first element is the size of the int array

  return result;

}

bigint to_base_10(int *input, int length){
  char zero[] = "0", twosev[]= "27", mul[]="1", temp[190] ;
  bigint result = new(zero);
  bigint multiplier = new(mul);
  bigint tw = new(twosev);
  bigint in;
  for (int i=length-1; i>=0; i--){
    snprintf(temp, 190,"%d", input[i]);
    in = new(temp);
    result = add(result, mult(in, multiplier));
    multiplier = mult(multiplier, tw);
  }
  return result;
}

bigint minus(bigint x){
  char temp[190], ans[190];
  int temp1[190] = { 0 };
  strcpy(temp, x.value);
  for (int i=0; i<x.len ;i++){
    temp1[i] = temp[i] - '0';
  }
  if(temp1[0] == 0)
    for(int i = 0; i<x.len; i++){
      if(temp1[i] == 0)
        temp1[i] = 9;
      else{
        temp1[i] = temp1[i] - 1;
        break;
      }
    }
  else
    temp1[0] -=1;

  int len = 0;
  for (int i=x.len; i>=0; i--){
    if (temp1[i] > 0){
      len = i+1;
      break;
    }
  }
  for (int i = 0; i<len; i++){
    ans[i] = temp1[i] + '0';
  }
  ans[len] = '\0';
  x.len = len;
  strcpy(x.value, ans);
  return x;

}

bigint phi(bigint p, bigint q){
}


int get_b27_index(char input){
  char string[27] = "0123456789abcdefghijklmnopq";
  char *i = strchr(string, input);
  int ans = 0;
  if (i){
    ans = i - string;
  }
  else{
    ans = -1;
  }
  return ans;
}



char to_base_27(char input){
  switch (input){
    case 'A':
      return '0';
    case 'B':
      return '1';
    case 'C':
      return '2';
    case 'D':
      return '3';
    case 'E':
      return '4';
    case 'F':
      return '5';
    case 'G':
      return '6';
    case 'H':
      return '7';
    case 'I':
      return '8';
    case 'J':
      return '9';
    case 'K':
      return 'a';
    case 'L':
      return 'b';
    case 'M':
      return 'c';
    case 'N':
      return 'd';
    case 'O':
      return 'e';
    case 'P':
      return 'f';
    case 'Q':
      return 'g';
    case 'R':
      return 'h';
    case 'S':
      return 'i';
    case 'T':
      return 'j';
    case 'U':
      return 'k';
    case 'V':
      return 'l';
    case 'W':
      return 'm';
    case 'X':
      return 'n';
    case 'Y':
      return 'o';
    case 'Z':
      return 'p';
    case ' ':
      return 'q';
    }
  }

int bigint_to_int(bigint x){     //ONLY RETURN WHEN THE BIGINT CAN FIT, for ease of use
  char buf[10];
  strcpy(buf, x.value);
  int result = atoi(buf);
  return (result);
}

  char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}
