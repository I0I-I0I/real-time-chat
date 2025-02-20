import cls from "@/utils/cls"
import styles from "./ChatsList.module.css"
import { ChatsItem } from "./chatsItem/ChatsItem"
import { IChat } from "@/types"
// import ChatService from "@/api/ChatService"

interface ChatsListProps {
    className?: string
    onClick?: (chat: IChat) => void
    data: IChat[] | null
}

export const ChatsList = ({
    className = "",
    onClick = () => {},
    data
}: ChatsListProps): JSX.Element => {
    const addChatData: IChat = {
        id: 0,
        name: "Add Chat",
        createdAt: "",
        img: "/add_chat.svg"
    }

    return (
        <ul className={cls(styles.list, className)}>
            <ChatsItem onClick={onClick} data={addChatData} index={null} key={0} />
            { data && data.map((item: IChat, index: number): JSX.Element => (
                <ChatsItem data={item} index={index+1} key={index} onClick={onClick} />
            ))}
        </ul>
    )
}
