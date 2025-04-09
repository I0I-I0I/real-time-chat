import cls from "@/utils/cls"
import styles from "./ChatsList.module.css"
import { ChatsItem } from "./chatsItem/ChatsItem"
import { IChat } from "@/types"
// import ChatService from "@/api/ChatService"

interface ChatsListProps {
    className?: string
    onClick?: (chat: IChat) => void
    createNewChat?: (name: string) => void
    removeChat: (chatId: number) => void
    data: IChat[]
}

export const ChatsList = ({
    className = "",
    onClick = () => {},
    removeChat,
    data = []
}: ChatsListProps): JSX.Element => {
    return (
        <>
            <ul className={cls(styles.list, className)}>
                { data.slice().reverse().map((item: IChat, index: number): JSX.Element => (
                    <ChatsItem data={item} index={index+1} key={index} onClick={onClick} onClickRemove={removeChat} />
                ))}
            </ul>
        </>
    )
}
