#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char to_base_27(char input);
char *strrev(char *str);
int get_b27_index(char input);
char *back_to_text(int* input);


typedef struct{
  char value[193];
  int len;
} bigint;

int* back_to_27(bigint input);
bigint mult(bigint x, bigint y);
bigint new(char *input);
bigint minus(bigint x);
bigint to_base_10(int *input, int length);
bigint add(bigint x, bigint y);
bigint modulo(bigint x, bigint m);
bigint encrypt(bigint m, bigint e, bigint n);
bigint divide(bigint x, bigint y);
bigint subtract(bigint x, bigint y);
bigint compute_for_d(bigint e, bigint n);




void main(){
  FILE *fp;
  FILE *fp1;
  fp1 =fopen("201357296.txt", "w+");
  fp = fopen("mp2.txt", "r");

  int ctr = 0;
  char twosev[] = "27";
  bigint tw = new(twosev);
  char pbuf[199], qbuf[199], ebuf[199];
  char buffer[199];
  int m[190], size;
  int *b27;
  bigint base10_message;
  bigint n, p, q, e, phin, minusp, minusq, c, d;
  char line[255], *out;
  fgets(line, 255, fp);
  char sender[10], reciever[10], message[255];
  while (sscanf(line, "CASE %d", &ctr) == 1){
    printf("%s", line);
    fprintf(fp1,"%s", line);
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
        b27 = back_to_27(c);
        printf("%d!", b27[0]);
        out = back_to_text(b27);
        printf("Message is: %s", out);
        fprintf(fp1,"Alice sent: %s\n", out);
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
        d = compute_for_d(n, e);
        c = encrypt(base10_message, d, n);
        b27 = back_to_27(c);
        printf("%d!", b27[0]);
        out = back_to_text(b27);
        printf("Message is: %s", out);
        fprintf(fp1,"Bob received: %s\n", out);

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
  fclose(fp);
  fclose(fp1);
}

bigint compute_for_d(bigint e, bigint n){
  bigint n0 = n, t, q;
  char zero[] = "0", three[] = "3", one[] = "1", temp[199];
  bigint x0 = new(zero), x1 = new(one);
  if(strcmp(n.value, "1") == 0)
    return n;
  strcpy(temp, e.value);
  strrev(temp);
  while(atoi(temp) > 1){
    printf("haha, %s\n", temp);
    q = divide(e,n);
    t = n;
    n = modulo(e, n);
    e = t;
    t = x0;
    x0 = subtract(x1, mult(q, x0));
    x1 = t;
    strcpy(temp, e.value);
    strrev(temp);
  }
  strcpy(temp, x1.value);
  strrev(temp);
  if (atoi(temp) < 0)
    x1 = add(x1, n0);
  //x1 = new(three);
  printf("D IS %s", x1.value );
  return x1;
}

bigint new(char *input){
  bigint x;
  x.len = strlen(input);
  strcpy(x.value, strrev(input));
  return x;
}

bigint encrypt(bigint m, bigint e, bigint n){
  char buf[199], buf1[199];
  strcpy(buf, m.value);
  strcpy(buf1, e.value);
  strrev(buf);
  strrev(buf1);
  char *one = "1", *two = "2";
  bigint t = new(one);
  bigint temp1;
  bigint z = new(two);
  bigint c, temp = e;
  m = modulo(m, n);
  while(strcmp(temp1.value, "0") != 0){
    temp1 = modulo(e,z);
    if(strcmp(temp1.value, "1") == 0)
      t = modulo(mult(t,m),n);
    e = divide(e, z);
    m = modulo(mult(m,m),n);
  }
  strcpy(buf, t.value);
  strrev(buf);
  printf("Base 10 C: %s, %d\n", buf, t.len);
  return t;
}
bigint subtract(bigint x, bigint y){
  char r[199], buf[199];
  int i = 0, j = 0, k =0, borrow = 0, maxlen = 0;
  int result[199] = { 0 };
  bigint res;
  maxlen = (x.len > y.len) ? x.len : y.len;
  while(i<x.len && j <y.len){
    result[k] = ((x.value[k] - '0') - (y.value[k] - '0')) - borrow;
    if (result[k] < 0){
      //printf("%d", result[k]);
      result[k] +=10;
      borrow = 1;
    }
    else
      borrow = 0;
    i++;
    j++;
    k++;
  }
  //printf("%d%d", result[0], result[1]);
  while(i<x.len){
    result[k]=(x.value[i]-'0')-borrow;
    if(result[k]<0){
      result[k]+=10;
      borrow=1;
    }
    else
      borrow=0;
    i++;
    k++;
  }

  while(j<y.len){
    result[k]=(y.value[j]-'0')-borrow;
    if(result[k]<0){
      result[k]+=10;
      borrow=1;
    }
    else
      borrow=0;
    j++;
    k++;
  }

  for (int ctr = maxlen - 1; ctr>0; ctr--){
    if(result[ctr] == 0)
      maxlen = ctr;
      break;
  }

  for(int ctr = 0; ctr<maxlen; ctr++){
    r[ctr] = result[ctr] + '0';
  }
  r[maxlen] = '\0';
  strcpy(res.value, r);
  res.len = maxlen;
  strcpy(buf, res.value);
  strrev(buf);
  //printf("Difference: %s\n", buf);
  return res;
}

bigint divide(bigint x, bigint y){
  bigint result;

  char r[199], buf[199], buf1[199];
  strcpy(buf, x.value);
  strrev(buf);
  strcpy(buf1, y.value);
  strrev(buf1);
  printf("\n%s divide by %s: ", buf, buf1);
  int i = 0;
  while(1){
    if (x.len < y.len)
      break;
    else if (x.len == y.len && x.value[x.len-1] - '0' < y.value[y.len-1] - '0')
      break;
    else{
      x = subtract(x, y);
    }
    i++;
  }
  printf("Quotient: %d\n", i);
  snprintf(r, 199,"%d", i);
  result = new(r);
  return result;
}

bigint modulo(bigint x, bigint m){
  bigint result;

  char r[199], buf[199], buf1[199];
  strcpy(buf, x.value);
  strrev(buf);
  strcpy(buf1, m.value);
  strrev(buf1);
  printf("\n%s Modulo %s: ", buf, buf1);
  int i = 0;
  while(1){
    if (x.len < m.len)
      break;
    else if (x.len == m.len && x.value[x.len-1] - '0' < m.value[m.len-1] - '0')
      break;
    else{
      x = subtract(x, m);
    }
    i++;
  }
  strcpy(buf, x.value);
  strrev(buf);
  printf("Modulo: %s\n", buf);
  return x;
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

int* back_to_27(bigint input){
  char twosev[] = "27";
  bigint tw = new(twosev);
  bigint temp1;
  int t[190];
  int j = 0;
  int *result = malloc(190 * sizeof(*result));
  bigint temp = input;
  int c = 0;
  while(strcmp(temp.value, "0") != 0){
    temp1 = modulo(temp, tw);
    strrev(temp1.value);
    t[c] = atoi(temp1.value);
    temp =  divide(temp, tw);
    c++; //hehe
  }

  j = 1;
  for (int i = c-1; i>=0; i--){
    result[j] = t[i];
    printf("cbb: %d\n", result[j]);
    j++;
  }
  result[0] = j; //first element is the size of the int array
  printf("%d!!HEHE",result[0]);
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
  result.len = strlen(result.value);
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

char *back_to_text(int* input){
  char string[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ", *buf = malloc(199 * sizeof(char));
  int input_length = input[0];
  for(int i = 0; i<input_length - 1; i++){
    buf[i] = string[input[i+1]];
  }
  buf[input_length] = '\0';
  return buf;
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
