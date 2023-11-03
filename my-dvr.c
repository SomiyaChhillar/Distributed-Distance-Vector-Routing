#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond4 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond5 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock5 = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t break_cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t break_cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t break_cond3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t break_cond4 = PTHREAD_COND_INITIALIZER;
pthread_cond_t break_cond5 = PTHREAD_COND_INITIALIZER;

int update1 = 0;
int update2 = 0;
int update3 = 0;
int update4 = 0;
int update5 = 0;

char* nodes[5] = {"Node E", "Node A", "Node B","Node C","Node D"};


int converged = 5;
int converged1 = 1;
int converged2 = 1;
int converged3 = 1;
int converged4 = 1;
int converged5 = 1;

int converged_round_number = 0;
int converged_round_flag = 0;

int node1[5] = {0,   0,  0,  0,  0};
int node2[5] = {0,   0,  0,  0,  0};
int node3[5] = {0,   0,  0,  0,  0};
int node4[5] = {0,   0,  0,  0,  0};
int node5[5] = {0,   0,  0,  0,  0};

int previous_node1[5] = {0,   0,  0,  0,  0};
int previous_node2[5] = {0,   0,  0,  0,  0};
int previous_node3[5] = {0,   0,  0,  0,  0};
int previous_node4[5] = {0,   0,  0,  0,  0};
int previous_node5[5] = {0,   0,  0,  0,  0};

int node1_neighbours_size = 0;
int node2_neighbours_size = 0;
int node3_neighbours_size = 0;
int node4_neighbours_size = 0;
int node5_neighbours_size = 0;

int * node1_neighbours = NULL;
int * node2_neighbours = NULL;
int * node3_neighbours = NULL;
int * node4_neighbours = NULL;
int * node5_neighbours = NULL;

int TRUE = 1;
int round_count = 0;
int started = 0;

void read_file(int node1[], int node2[], int node3[], int node4[], int node5[])
{
  FILE *fp;
  fp = fopen("input.txt", "r");

  if(fp == NULL) 
  {
    printf("Error opening file.\n");
    return;
  }

  for(int k=0;k<5;k++)
  {
    fscanf(fp, "%d", &node1[k]);
    if(k!=0)
    {
      if(node1[k] == 0)
      {
        node1[k] = 999;
      }
    }
  }
 
  for(int k=0;k<5;k++)
  {
    fscanf(fp, "%d", &node2[k]);
    if(k!=1)
    {
      if(node2[k] == 0)
      {
        node2[k] = 999;
      }
    }
  }
  
  for(int k=0;k<5;k++)
  {
    fscanf(fp, "%d", &node3[k]);
    if(k!=2)
    {
      if(node3[k] == 0)
      {
        node3[k] = 999;
      }
    }
  }

  for(int k=0;k<5;k++)
  {
    fscanf(fp, "%d", &node4[k]);
    if(k!=3)
    {
      if(node4[k] == 0)
      {
        node4[k] = 999;
      }
    }
  }

  for(int k=0;k<5;k++)
  {
    fscanf(fp, "%d", &node5[k]);
    if(k!=4)
    {
      if(node5[k] == 0)
      {
        node5[k] = 999;
      }
    }
  }

  fclose(fp);
}

void get_neighbours_size(int * node1_neighbours_size, int *node2_neighbours_size, int *node3_neighbours_size, int * node4_neighbours_size, int *node5_neighbours_size,
          int node1[], int node2[], int node3[], int node4[], int node5[])
{
    int num1=0,num2=0,num3=0,num4=0,num5=0;  
    for(int k=0;k<5;k++)
    {
      if (node1[k]>0 && node1[k]<999)
      {
        num1 += 1;
      }
    }
    
    for(int k=0;k<5;k++)
    {
      if (node2[k]>0 && node2[k]<999)
      {
        num2 += 1;
      }
    }
    
    for(int k=0;k<5;k++)
    {
      if (node3[k]>0 && node3[k]<999)
      {
        num3 += 1;
      }
    }
    
    for(int k=0;k<5;k++)
    {
      if (node4[k]>0 && node4[k]<999)
      {
        num4 += 1;
      }
    }
    
    for(int k=0;k<5;k++)
    {
      if (node5[k]>0 && node5[k]<999)
      {
        num5 += 1;
      }
    }

    *node1_neighbours_size = num1;
    *node2_neighbours_size = num2;
    *node3_neighbours_size = num3;
    *node4_neighbours_size = num4;
    *node5_neighbours_size = num5;
}

void get_neighbours(int * neigh1, int *neigh2, int *neigh3, int * neigh4, int *neigh5,
          int node1[], int node2[], int node3[], int node4[], int node5[])
{
  

    int node1_index = 0;
    for(int z = 0; z< 5; z++)
    {
      if (node1[z]>0 && node1[z]<999)
      {
        neigh1[node1_index] = z+1;
        node1_index += 1;
      }
    }

    int node2_index = 0;
    for(int z = 0; z< 5; z++)
    {
      if (node2[z]>0 && node2[z]<999)
      {
        neigh2[node2_index] = z+1;
        node2_index += 1;
      }
    }

    int node3_index = 0;
    for(int z = 0; z< 5; z++)
    {
      if (node3[z]>0 && node3[z]<999)
      {
        neigh3[node3_index] = z+1;
        node3_index += 1;
      }
    }

    int node4_index = 0;
    for(int z = 0; z< 5; z++)
    {
      if (node4[z]>0 && node4[z]<999)
      {
        neigh4[node4_index] = z+1;
        node4_index += 1;
      }
    }

    int node5_index = 0;
    for(int z = 0; z< 5; z++)
    {
      if (node5[z]>0 && node5[z]<999)
      {
        neigh5[node5_index] = z+1;
        node5_index += 1;
      }
    }
}

void assign_node_values(int * node1, int values[])
{
  for(int p=0; p<5;p++)
  {
    node1[p] = values[p];
  }
}

void assign_neighbour_values(int * neighbour, int values[], int len)
{
  for(int p=0; p<len;p++)
  {
    neighbour[p] = values[p];
  }
}

int compare_nodes(int * node1, int values[])
{
  for(int p=0; p<5;p++)
  {
    if(node1[p] != values[p])
      return 0;
  }
  return 1;
}

void print_dv_matrix(int * node1, int * node2,int * node3,int * node4,int * node5)
{
  for(int k=0;k<5;k++)
  {
    printf("%d\t",node1[k] );
  }
  printf("\n");
  for(int k=0;k<5;k++)
  {
    printf("%d\t",node2[k] );
  }
  printf("\n");
  for(int k=0;k<5;k++)
  {
    printf("%d\t",node3[k] );
  }
  printf("\n");
  for(int k=0;k<5;k++)
  {
    printf("%d\t",node4[k] );
  }
  printf("\n");
  for(int k=0;k<5;k++)
  {
    printf("%d\t",node5[k] );
  }
  printf("\n");

}

void * threadMethod1(void *arg)
{
  if(converged <= 0)
  {
    return NULL;
  }
  

  do{
    pthread_mutex_lock(&lock1);
   
    pthread_cond_wait(&cond1, &lock1);


    if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
    {
        if(converged_round_flag != 1)
        {
          converged_round_number = round_count;
          converged_round_flag = 1;
        }
    }

    else
    {
      converged_round_flag = 0;
    }

    if(started == 0)
      started = 1;

    if(update1 == 1)
    {
      printf("Node A received DV from %s\n", nodes[round_count%5]);

      int curr_node_num = 0;
      int prev_node_num = 4;
      if(round_count%5>0)
        prev_node_num = (round_count%5)-1;

      int i = 0;
      int new_dist = 0;
      int flag_converged = 0;

      int * prev_node = NULL;

      switch(prev_node_num)
      {
        case 1:
          prev_node = node2;
          break;
        case 2:
          prev_node = node3;
          break;
        case 3:
          prev_node = node4;
          break;
        case 4:
          prev_node = node5;
          break;
      }

      for(i=0;i<5;i++)
      {
        if((i != prev_node_num) && (i != curr_node_num))
        {
          new_dist = node1[prev_node_num] + prev_node[i];
          if(new_dist < node1[i])
          {
            node1[i] = new_dist;
            flag_converged = 1;
          }
        }
      }

      if(flag_converged == 0)
      {
        converged1 = 0;
        printf("No change in DV at Node A\n");
      }
      else
      {
        converged1 = 1;
        printf("New DV at Node A is -\t");
        for(i=0;i<5;i++)
          printf("%d\t", node1[i]);
        printf("\n");
      }

      update1 = 0;
      sleep(1);
    }

    else
    {
      if(converged == 0)
      {
        return NULL;
      }
      round_count += 1;
      if(round_count == 1)
        printf("\nRound %d: %s\n",round_count, nodes[round_count%5]);

      else
        printf("--------------------------------\nRound %d: %s\n",round_count, nodes[round_count%5]);

      printf("Current DV matrix: \n");
      print_dv_matrix(node1, node2,node3,node4,node5);
      printf("Last DV matrix: \n");
      print_dv_matrix(previous_node1, previous_node2,previous_node3,previous_node4,previous_node5);
      if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
        printf("Updated from last DV matrix or the same? Not Updated\n");
      else
        printf("Updated from last DV matrix or the same? Updated\n");
      assign_node_values(previous_node1,node1);
      assign_node_values(previous_node2,node2);
      assign_node_values(previous_node3,node3);
      assign_node_values(previous_node4,node4);
      assign_node_values(previous_node5,node5);

      for(int j=0; j<node1_neighbours_size;j++)
      {
        switch(node1_neighbours[j])
        {
        case 2:
          sleep(1);
          update2 = 1;
          printf("\nSending DV to node B\n");
          pthread_cond_signal(&cond2);
          break;
        case 3:
          sleep(1);
          update3 = 1;
          printf("\nSending DV to node C\n");
          pthread_cond_signal(&cond3);
          break;
        case 4:
          sleep(1);
          update4 = 1;
          printf("\nSending DV to node D\n");
          pthread_cond_signal(&cond4);
          break;
        case 5:
          sleep(1);
          update5 = 1;
          printf("\nSending DV to node E\n");
          pthread_cond_signal(&cond5);
          break;
        }
      }

      sleep(3);
      pthread_cond_signal(&cond2);
    }

    pthread_mutex_unlock(&lock1);

    
  }while(TRUE);
  //pthread_exit(NULL);
  return NULL;
}


void * threadMethod2(void *arg)
{
  if(converged <= 0)
  {
    return NULL;
  }
  do
  {
    pthread_mutex_lock(&lock2);
    pthread_cond_wait(&cond2, &lock2);

    /*if(converged1 == 0 && converged2 == 0 && converged3 == 0 && converged4 == 0 && converged5 == 0)
    {
      if(converged_round_flag != 1)
      {
        converged_round_number = round_count;
        converged_round_flag = 1;
      }
    }
    else
    {
      converged_round_flag = 0;
    }*/
    if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
    {
        if(converged_round_flag != 1)
        {
          converged_round_number = round_count;
          converged_round_flag = 1;
        }
    }
    else
    {
      converged_round_flag = 0;
    }
      
    if(update2 == 1)
    {
      printf("Node B received DV from %s\n", nodes[round_count%5]);

      int curr_node_num = 1;
      int prev_node_num = 4;
      if(round_count%5>0)
        prev_node_num = (round_count%5)-1;

      int i = 0;
      int new_dist = 0;
      int flag_converged = 0;

      int * prev_node = NULL;

      switch(prev_node_num)
      {
        case 0:
          prev_node = node1;
          break;
        case 2:
          prev_node = node3;
          break;
        case 3:
          prev_node = node4;
          break;
        case 4:
          prev_node = node5;
          break;
      }
      
      for(i=0;i<5;i++)
      {
        if((i != prev_node_num) && (i != curr_node_num))
        {
          new_dist = node2[prev_node_num] + prev_node[i];
          if(new_dist < node2[i])
          {
            node2[i] = new_dist;
            flag_converged = 1;
          }
        }
      }
      
      if(flag_converged == 0)
      {
        converged2 = 0;
        printf("No change in DV at Node B\n");
      }
      else
      {
        converged2 = 1;
        printf("New DV at Node B is -\t");
        for(i=0;i<5;i++)
          printf("%d\t", node2[i]);
        printf("\n");
      }

      update2 = 0;
      sleep(1);
    }

    else
    {
      if(converged == 0)
      {
        return NULL;
      }
      round_count += 1;
      printf("--------------------------------\nRound %d: %s\n",round_count, nodes[round_count%5]);

      printf("Current DV matrix: \n");
      print_dv_matrix(node1, node2,node3,node4,node5);
      printf("Last DV matrix: \n");
      print_dv_matrix(previous_node1, previous_node2,previous_node3,previous_node4,previous_node5);
      if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
        printf("Updated from last DV matrix or the same? Not Updated\n");
      else
        printf("Updated from last DV matrix or the same? Updated\n");
      assign_node_values(previous_node1,node1);
      assign_node_values(previous_node2,node2);
      assign_node_values(previous_node3,node3);
      assign_node_values(previous_node4,node4);
      assign_node_values(previous_node5,node5);

      for(int j=0; j<node2_neighbours_size;j++)
      {
        switch(node2_neighbours[j])
        {
        case 1:
          sleep(1);
          update1 = 1;
          printf("\nSending DV to node A\n");
          pthread_cond_signal(&cond1);
          break;
        case 3:
          sleep(1);
          update3 = 1;
          printf("\nSending DV to node C\n");
          pthread_cond_signal(&cond3);
          break;
        case 4:
          sleep(1);
          update4 = 1;
          printf("\nSending DV to node D\n");
          pthread_cond_signal(&cond4);
          break;
        case 5:
          sleep(1);
          update5 = 1;
          printf("\nSending DV to node E\n");
          pthread_cond_signal(&cond5);
          break;
        }
      }


      sleep(3);
      pthread_cond_signal(&cond3);
    }

    pthread_mutex_unlock(&lock2);

    
    
  }while(TRUE);  return NULL;
}

void * threadMethod3(void *arg)
{


  do
  {
    pthread_mutex_lock(&lock3);
    pthread_cond_wait(&cond3, &lock3);

    /*if(converged1 == 0 && converged2 == 0 && converged3 == 0 && converged4 == 0 && converged5 == 0)
    {
      if(converged_round_flag != 1)
      {
        converged_round_number = round_count;
        converged_round_flag = 1;
      }
    }
    else
    {
      converged_round_flag = 0;
    }*/
    if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
    {
        if(converged_round_flag != 1)
        {
          converged_round_number = round_count;
          converged_round_flag = 1;
        }
    }
    else
    {
      converged_round_flag = 0;
    }

    if(update3 == 1)
    {
      printf("Node C received DV from %s\n", nodes[round_count%5]);

      int curr_node_num = 2;
      int prev_node_num = 4;
      if(round_count%5>0)
        prev_node_num = (round_count%5)-1;

      int i = 0;
      int new_dist = 0;
      int flag_converged = 0;

      int * prev_node = NULL;

      switch(prev_node_num)
      {
        case 0:
          prev_node = node1;
          break;
        case 1:
          prev_node = node2;
          break;
        case 3:
          prev_node = node4;
          break;
        case 4:
          prev_node = node5;
          break;
      }

      for(i=0;i<5;i++)
      {
        if((i != prev_node_num) && (i != curr_node_num))
        {
          new_dist = node3[prev_node_num] + prev_node[i];
          if(new_dist < node3[i])
          {
            node3[i] = new_dist;
            flag_converged = 1;
          }
        }
      }

      if(flag_converged == 0)
      {
        converged3 = 0;
        printf("No change in DV at Node C\n");
      }

      else
      {
        converged3 = 1;
        printf("New DV at Node C is -\t");
        for(i=0;i<5;i++)
          printf("%d\t", node3[i]);
        printf("\n");
      }
      update3 = 0;
      sleep(1);
    }

    else
    {
      if(converged == 0)
      {
        return NULL;
      }
      round_count += 1;
      printf("--------------------------------\nRound %d: %s\n",round_count, nodes[round_count%5]);


      printf("Current DV matrix: \n");
      print_dv_matrix(node1, node2,node3,node4,node5);
      printf("Last DV matrix: \n");
      print_dv_matrix(previous_node1, previous_node2,previous_node3,previous_node4,previous_node5);
      if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
        printf("Updated from last DV matrix or the same? Not Updated\n");
      else
        printf("Updated from last DV matrix or the same? Updated\n");
      assign_node_values(previous_node1,node1);
      assign_node_values(previous_node2,node2);
      assign_node_values(previous_node3,node3);
      assign_node_values(previous_node4,node4);
      assign_node_values(previous_node5,node5);

      for(int j=0; j<node3_neighbours_size;j++)
      {
        switch(node3_neighbours[j])
        {
        case 1:
          sleep(1);
          update1 = 1;
          printf("\nSending DV to node A\n");
          pthread_cond_signal(&cond1);
          break;
        case 2:
          sleep(1);
          update2 = 1;
          printf("\nSending DV to node B\n");
          pthread_cond_signal(&cond2);
          break;
        case 4:
          sleep(1);
          update4 = 1;
          printf("\nSending DV to node D\n");
          pthread_cond_signal(&cond4);
          break;
        case 5:
          sleep(1);
          update5 = 1;
          printf("\nSending DV to node E\n");
          pthread_cond_signal(&cond5);
          break;
        }
      }

      sleep(3);
      pthread_cond_signal(&cond4);
    }


    pthread_mutex_unlock(&lock3);


  }while(TRUE);
  //pthread_exit(NULL);
  return NULL;
}

void * threadMethod4(void *arg)
{
  
  if(converged <= 0)
  {
    return NULL;
  }
  do
  {
    pthread_mutex_lock(&lock4);
    pthread_cond_wait(&cond4, &lock4);

    if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
    {
        if(converged_round_flag != 1)
        {
          converged_round_number = round_count;
          converged_round_flag = 1;
        }
    }
    else
    {
      converged_round_flag = 0;
    }

    if(update4 == 1)
    {  
      printf("Node D received DV from %s\n", nodes[round_count%5]);

      int curr_node_num = 3;
      int prev_node_num = 4;
      if(round_count%5>0)
        prev_node_num = (round_count%5)-1;

      int i = 0;
      int new_dist = 0;
      int flag_converged = 0;

      int * prev_node = NULL;

      switch(prev_node_num)
      {
        case 0:
          prev_node = node1;
          break;
        case 1:
          prev_node = node2;
          break;
        case 2:
          prev_node = node3;
          break;
        case 4:
          prev_node = node5;
          break;
      }
      for(i=0;i<5;i++)
      {
        if((i != prev_node_num) && (i != curr_node_num))
        {
          new_dist = node4[prev_node_num] + prev_node[i];
          if(new_dist < node4[i])
          {
            node4[i] = new_dist;
            flag_converged = 1;
          }
        }
      }

      if(flag_converged == 0)
      {
        converged4 = 0;
        printf("No change in DV at Node D\n");
      }
      else
      {
        converged4 = 1;
        printf("New DV at Node D is -\t");
        for(i=0;i<5;i++)
          printf("%d\t", node4[i]);
        printf("\n");
      }

      update4 = 0;
      sleep(1);
    }

    else
    { 
      if(converged == 0)
      {
        return NULL;
      }
      round_count += 1;
      printf("--------------------------------\nRound %d: %s\n",round_count, nodes[round_count%5]);

      printf("Current DV matrix: \n");
      print_dv_matrix(node1, node2,node3,node4,node5);
      printf("Last DV matrix: \n");
      print_dv_matrix(previous_node1, previous_node2,previous_node3,previous_node4,previous_node5);
      if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
        printf("Updated from last DV matrix or the same? Not Updated\n");
      else
        printf("Updated from last DV matrix or the same? Updated\n");
      assign_node_values(previous_node1,node1);
      assign_node_values(previous_node2,node2);
      assign_node_values(previous_node3,node3);
      assign_node_values(previous_node4,node4);
      assign_node_values(previous_node5,node5);

      for(int j=0; j<node4_neighbours_size;j++)
      {
        switch(node4_neighbours[j])
        {
        case 1:
          sleep(1);
          update1 = 1;
          printf("\nSending DV to Node A\n");
          pthread_cond_signal(&cond1);
          break;
        case 2:
          sleep(1);
          update2 = 1;
          printf("\nSending DV to Node B\n");
          pthread_cond_signal(&cond2);
          break;
        case 3:
          sleep(1);
          update3= 1;
          printf("\nSending DV to Node C\n");
          pthread_cond_signal(&cond3);
          break;
        case 5:
          sleep(1);
          update5 = 1;
          printf("\nSending DV to Node E\n");
          pthread_cond_signal(&cond5);
          break;
        }
      }

      sleep(3);
      pthread_cond_signal(&cond5);
    }

    pthread_mutex_unlock(&lock4);


  }while(TRUE);
  
  return NULL;
}

void * threadMethod5(void *arg)
{

  if(converged <= 0)
  {
    return NULL;
  }

  if(started == 0)
  {
    pthread_cond_signal(&cond1);
  }
  do
  {
    pthread_mutex_lock(&lock5);
    pthread_cond_wait(&cond5, &lock5);

    if(converged1 == 0 && converged2 == 0 && converged3 == 0 && converged4 == 0 && converged5 == 0)
    {
      TRUE = 0;
      converged = 0;

      pthread_cond_signal(&cond1);
      pthread_cond_signal(&cond2);
      pthread_cond_signal(&cond3);
      pthread_cond_signal(&cond4);
      break;
    }
    

    if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
    {
        if(converged_round_flag != 1)
        {
          converged_round_number = round_count;
          converged_round_flag = 1;
        }
    }
    else
    {
      converged_round_flag = 0;
    }

    if(update5 == 1)
    { 
      printf("Node E received DV from %s\n", nodes[round_count%5]);

      int curr_node_num = 4;
      int prev_node_num = 4;
      if(round_count%5>0)
        prev_node_num = (round_count%5)-1;

      int i = 0;
      int new_dist = 0;
      int flag_converged = 0;

      int * prev_node = NULL;

      switch(prev_node_num)
      {
        case 0:
          prev_node = node1;
          break;
        case 1:
          prev_node = node2;
          break;
        case 2:
          prev_node = node3;
          break;
        case 3:
          prev_node = node4;
          break;
      }
      for(i=0;i<5;i++)
      {
        if((i != prev_node_num) && (i != curr_node_num))
        {
          new_dist = node5[prev_node_num] + prev_node[i];
          if(new_dist < node5[i])
          {
            node5[i] = new_dist;
            flag_converged = 1;
          }
        }
      }

      if(flag_converged == 0)
      {
        converged5 = 0;
        printf("No change in DV at Node E\n");
      }
      else
      {
        converged5 = 1;
        printf("New DV at Node E is -\t");
        for(i=0;i<5;i++)
          printf("%d\t", node5[i]);
        printf("\n");
      }

      update5 = 0;
      sleep(1);
    }

    else
    {
      if(converged == 0)
      {
        return NULL;
      }
      round_count += 1;
      printf("--------------------------------\nRound %d: %s\n",round_count, nodes[round_count%5]);

      printf("Current DV matrix: \n");
      print_dv_matrix(node1, node2,node3,node4,node5);
      printf("Last DV matrix: \n");
      print_dv_matrix(previous_node1, previous_node2,previous_node3,previous_node4,previous_node5);
      if(compare_nodes(node1,previous_node1) && compare_nodes(node2,previous_node2) && compare_nodes(node3,previous_node3) && compare_nodes(node4,previous_node4) && compare_nodes(node5,previous_node5))
        printf("Updated from last DV matrix or the same? Not Updated\n");
      else
        printf("Updated from last DV matrix or the same? Updated\n");
      assign_node_values(previous_node1,node1);
      assign_node_values(previous_node2,node2);
      assign_node_values(previous_node3,node3);
      assign_node_values(previous_node4,node4);
      assign_node_values(previous_node5,node5);

      for(int j=0; j<node5_neighbours_size;j++)
      {
        switch(node5_neighbours[j])
        {
        case 1:
          sleep(1);
          update1 = 1;
          printf("\nSending DV to Node A\n");
          pthread_cond_signal(&cond1);
          break;
        case 2:
          sleep(1);
          update2 = 1;
          printf("\nSending DV to Node B\n");
          pthread_cond_signal(&cond2);
          break;
        case 3:
          sleep(1);
          update3= 1;
          printf("\nSending DV to Node C\n");
          pthread_cond_signal(&cond3);
          break;
        case 4:
          sleep(1);
          update4 = 1;
          printf("\nSending DV to Node D\n");
          pthread_cond_signal(&cond4);
          break;
        }
      }

      sleep(3);
      pthread_cond_signal(&cond1);
    }

    pthread_mutex_unlock(&lock5);

  }while(TRUE);
  return NULL;
}

int main(void)
{

  int read_node_1[5] = {0,   0,  0,  0,  0};
  int read_node_2[5] = {0,   0,  0,  0,  0};
  int read_node_3[5] = {0,   0,  0,  0,  0};
  int read_node_4[5] = {0,   0,  0,  0,  0};
  int read_node_5[5] = {0,   0,  0,  0,  0};

  read_file(read_node_1, read_node_2, read_node_3, read_node_4, read_node_5);

  assign_node_values(node1, read_node_1);
  assign_node_values(node2, read_node_2);
  assign_node_values(node3, read_node_3);
  assign_node_values(node4, read_node_4);
  assign_node_values(node5, read_node_5);

  //print_dv_matrix(node1, node2,node3,node4,node5);

  assign_node_values(previous_node1, read_node_1); 
  assign_node_values(previous_node2, read_node_2);
  assign_node_values(previous_node3, read_node_3);
  assign_node_values(previous_node4, read_node_4);
  assign_node_values(previous_node5, read_node_5);

  int * neigh1 = NULL;
  int * neigh2 = NULL;
  int * neigh3 = NULL;
  int * neigh4 = NULL;
  int * neigh5 = NULL;

  node1_neighbours_size = 0;
  node2_neighbours_size = 0;
  node3_neighbours_size = 0;
  node4_neighbours_size = 0;
  node5_neighbours_size = 0;

  get_neighbours_size(&node1_neighbours_size,&node2_neighbours_size,&node3_neighbours_size,&node4_neighbours_size,&node5_neighbours_size,
    node1, node2, node3, node4, node5);



  node1_neighbours = malloc(sizeof(int)*(node1_neighbours_size));
  node2_neighbours = malloc(sizeof(int)*(node2_neighbours_size));
  node3_neighbours = malloc(sizeof(int)*(node3_neighbours_size));
  node4_neighbours = malloc(sizeof(int)*(node4_neighbours_size));
  node5_neighbours = malloc(sizeof(int)*(node5_neighbours_size));

  memset(node1_neighbours, 0, node1_neighbours_size);
  memset(node2_neighbours, 0, node2_neighbours_size);
  memset(node3_neighbours, 0, node3_neighbours_size);
  memset(node4_neighbours, 0, node4_neighbours_size);
  memset(node5_neighbours, 0, node5_neighbours_size);

  get_neighbours(node1_neighbours,node2_neighbours,node3_neighbours,node4_neighbours,node5_neighbours,node1, node2, node3, node4, node5);



  pthread_t tid1, tid2, tid3, tid4, tid5;

  if( pthread_create(&tid1, NULL, threadMethod1, NULL) != 0 )
        printf("Failed to create thread1\n");
  if( pthread_create(&tid2, NULL, threadMethod2, NULL) != 0 )
        printf("Failed to create thread2\n");
  if( pthread_create(&tid3, NULL, threadMethod3, NULL) != 0 )
        printf("Failed to create thread3\n");
  if( pthread_create(&tid4, NULL, threadMethod4, NULL) != 0 )
        printf("Failed to create thread4\n");
  if( pthread_create(&tid5, NULL, threadMethod5, NULL) != 0 )
        printf("Failed to create thread5\n");
  


 
  TRUE = 1;
    
  pthread_join(tid1,NULL);
  pthread_join(tid2,NULL);
  pthread_join(tid3,NULL);
  pthread_join(tid4,NULL);
  pthread_join(tid5,NULL);

  printf("\n--------------------------------\nFinal Output\n");

  print_dv_matrix(node1, node2,node3,node4,node5);

  printf("\nNumber of rounds till convergence = %d\n\n",converged_round_number );

 exit(0);
}