此demo包括ice的普通示例，Glacier2，IceGrid以及Glacier2和IceGrid组合的使用示例。

## 前提

- 编译ice文件

  ```bash
  slice2cpp hello.ice
  ```

  

## 普通示例

- 描述

  - 功能：服务器和客户端直连通讯；

- 流程

  - 服务器绑定监听端口和ip，启动服务；
  - 客户端指定服务器ip和端口，创建连接并通讯；

- 代码

  - SimpleServer.cpp
  - SimpleClient.cpp

- 步骤

  - 编译服务器并启动
  - 编译客户端并启动

  

## Glacier2示例

- 描述

  - 功能：客户端通过连接Glacier2服务，并指定服务器的ip和port，在Glacier2和服务器之间创建连接（Glacier2作为客户端），将消息通过Glacier2转发给服务器。

- 流程

  - 服务器绑定监听端口和ip，创建Adapter并加载servant，启动服务；
  - 启动Glacier2服务；
  - 客户端连接Glacier2服务，将数据发送Glacier2服务，并告诉Glacier2，服务器的ip和port以及ServantId；
  - Glacier2作为客户端，创建和服务器的连接，并将数据转发给服务器；

- 代码

  - Glacier2Server.cpp

  - Glacier2Client.cpp

- 配置文件

  - config.glacier2

- 步骤

  - 启动Glacier2路由

    ```bash
    glacier2router --Ice.Config=/home/luodahui/core/ice-demos/cpp98/Glacier2_Grid/config.glacier2
    ```

  - 编译服务器并启动

    ```bash
    g++ -oglacier2_server Hello.cpp HelloI.cpp Glacier2Server.cpp -lIce -lpthread && ./glacier2_server
    ```

  - 编译客户端并启动

    ```bash
    g++ -oglacier2_client Hello.cpp HelloI.cpp Glacier2Client.cpp -lIce -lGlacier2 -lpthread && ./glacier2_client
    ```

## IceGrid示例

- 描述

  - 功能：注册服务器的AdapterId和ServantId到IceGrid服务中，客户端通过服务器的AdapterId和ServantId找到服务器的ip和port，并创建和服务器的连接然后通讯；

- 流程

  - 启动IceGrid服务；
  - 服务器设置连接IceGrid服务的信息，并绑定ip（或localhost），创建AdapterId，并加载ServantId并将AdapterId和Servant注册到IceGrid服务；
  - 客户端设置连接IceGrid服务的信息，通过服务器对外暴露的AdapterId和ServantId找到服务器的ip和port；
  - 客户端创建和服务器的连接然后通讯，发送数据；

- 代码

  - GridServer.cpp

  - GridClient.cpp

- 配置文件

  - config.grid

- 步骤

  - 创建Grid注册数据库目录

    ```bash
    mkdir -p deploy/lmdb/registry
    ```

  - 启动Grid

    ```bash
    icegridregistry --Ice.Config=/home/luodahui/core/ice-demos/cpp98/Glacier2_Grid/config.grid
    ```

  - 编译服务器并启动

    ```bash
    g++ -ogrid_server Hello.cpp HelloI.cpp GridRegServer.cpp -lIce -lpthread && ./grid_server
    ```

  - 编译客户端并启动

    ```bash
    g++ -ogrid_client Hello.cpp HelloI.cpp GridRegClient.cpp -lIce -lIceGrid -lGlacier2 -lpthread && ./grid_client
    ```

## Glacier2和IceGrid组合示例

- 描述

  - 功能：服务器注册Id到IceGrid；客户端发送数据时，将服务器对外暴露的Id和数据推送给Glacier2服务，Glacier2通过配置Registry地址，从IceGrid找到服务器注册Id对应服务器的ip和port，创建和服务器的连接，并将数据转发给服务器，开始通讯；

- 流程

  - 服务器自动分配端口，并启动服务；

  - 将服务器的ip和端口绑定到AdapterId和ServantId上;

  - 客户端将要访问的服务器的AdapterId和ServantId告诉Glacier2服务;

  - Glacier2通过AdapterId和ServantId找到服务器的ip和端口;

  - Glacier2和服务器创建连接(Glacier2作为客户端)，并对客户端发送过来的数据进行转发;

- 代码

  - Glacier2GridServer.cpp

  - Glacier2GridClient.cpp

- 配置文件

  - IceGrid注册配置文件：config.gridregistry

    > 注意：localhost都改成ip地址。

  - Glacier2配置：config.glacier2

    > 配置文件中需要添加IceGrid服务的地址，目的是为了通过客户端提供的AdapterId和ServantId，在IceGrid服务找到对应的服务器ip和port。

- 步骤

  - 创建Grid注册数据库目录

    ```bash
    mkdir -p deploy/lmdb/registry
    ```

  - 启动Glacier2路由

    ```bash
    glacier2router --Ice.Config=/home/luodahui/core/ice-demos/cpp98/Glacier2_Grid/config.glacier2
    ```

  - 启动Grid

    ```bash
    icegridregistry --Ice.Config=/home/luodahui/core/ice-demos/cpp98/Glacier2_Grid/config.gridregistry
    ```

  - 编译服务器并启动

    ```bash
    g++ -oglacier2_grid_server Hello.cpp HelloI.cpp Glacier2GridRegServer.cpp -lIce -lpthread && ./glacier2_grid_server
    ```

  - 编译客户端并启动

    ```bash
    g++ -oglacier2_grid_client Hello.cpp HelloI.cpp Glacier2GridRegClient.cpp -lIce -lIceGrid -lGlacier2 -lpthread && ./glacier2_grid_client
    ```

