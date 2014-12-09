Exist
=====

《外存》本质上是一个内存级别的nosql。 外存提出了“面向变量”的数据访问思想，让用户只见变量，不见接口，省去了繁琐的接口调用。 外存拥有卓越的性能，单1client时，对于非持久化数据达到3.7万/秒吞吐，对于持久化数据达到2万/秒吞吐 client并发越多性能也就越高。 目前提供了int8、uint8、uint64、float、double等基础类型变量的操作,后续将实现map vector类型变量操作。

来看看面向变量的接口方式比传统数据接口方式要方便多少

场景1:假设服务上有3个逻辑：用户登录时，用户发表文章，用户发表评论时，要提升用户等级 。

传统数据库接口方式

申明数据库连接 db;

db.connect();

OnLogin()

{

申明变量 level

get(表名，"level"， level);

level++;

set(表名，"level", level);

}

OnSend()

{

申明变量 level

get(表名，"level"， level);

level++

set(表名，"level", level);
}

OnReply()

{

申明变量 level

get(表名，"level"， level);

level++

set(表名，"level", level);
}

面向变量的访问方式

Exist::int32 m_var("level");//申明变量

OnLogin()

{

var++;//数据已经在外存服务器上修改，下次访问将在新值基础上
}

OnSend()

{

var++;//数据已经在外存服务器上修改，下次访问将在新值基础上
}

OnReply()

{

var++;//数据已经在外存服务器上修改，下次访问将在新值基础上
}

就这么简单，用户直接看到一个个的变量，就像操作内存上的变量一样操作数据，根本不需要任何接口，外存由此得名

场景2:100万用户同时在线：用户A要查询用户B的资料

传统接口方式

m_otherNode;//其它结点/中转定位服务

std::map<...> m_userList;//已经登录当前结点的用户的列表

OnFindUser( char *username )

{

    map::iterater it = m_userList.find(username);

    if ( it == m_userList.end() )//用户未必再当前结点登录，

    {

            m_otherNode.Send("查询用户数据");//到其它结点，或其它定位服务中专查询

            m_otherNode.Recv();

            if (回应用户不存在) 

            {

                    回应用户不存在;

                    return;

            }

    }

    回应用户信息

    return;
};

复制代码

面向变量接口方式

Exist::map<...> m_userList;//已经登录的用户的列表,用户信息在外存上

OnFindUser( char *username )

{

    map::iterater it = m_userList.find(username);

    if ( it == m_userList.end() )//用户要么在外存中，要么不存在

    {

            回应，用户不存在;

            return;

    }

    回应用户信息;

    return;
};

省去了到其它结点查询的代码，省去了记录相关服务的连接对象，一套代码同时满足单结点与分布式需求，从此没有分布式一说

map与vector尚未实现，将在后续版本中实现，请期待
