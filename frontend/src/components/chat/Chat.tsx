import cls from "@/utils/cls"

import { Message } from "@/components/UI"
import { IMessage } from "@/types"

import { useUserStore } from "@/state/user"

import styles from "./Chat.module.css"

interface ChatProps {
    className?: string
    data: IMessage[] | null
}

export const Chat = ({
    className = "",
    data
}: ChatProps) => {
    const currentUser = useUserStore(state => state.data)

    return (
        <>
            { data ? (
                <ul className={cls(styles.messages, className)}>
                    { data.map((item: IMessage, index: number): JSX.Element => (
                        <Message key={index} variant={item.authorId === currentUser?.id ? "right" : "left"}>{item.text}</Message>
                    )) }
                </ul>
            ) : (
                    <div>It's empty</div>
                )
            }
        </>
    )
}
